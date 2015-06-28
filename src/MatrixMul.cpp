#include <random>
#include <iostream>
#include <set>
#include <cstddef>
#include <cstdio>
#include <cmath>
#include <stdexcept>

#include <boost/numeric/ublas/matrix.hpp>

#include <blaze/math/DynamicMatrix.h>

#include <mkl_cblas.h>

#include <blitz/array.h>

#include "MatrixMul.h"

template<class ForwardIt, class Generator>
void initialize(ForwardIt first, ForwardIt last, Generator & gen)
{
	std::uniform_real_distribution<> dis(0, 10);
	std::generate(first, last, [&] { return dis(gen); } );
}

template<class ForwardIt>
void initialize(ForwardIt first, ForwardIt last, const std::string & file, const std::array<uint32_t,2> & sizes)
{
	FILE * _file = fopen(file.c_str(), "r");
	if( !_file ) {
		throw std::runtime_error("Fatal error! File " + file + " can't be opened\n");
	}

	double * matrix = new double[sizes[0] * sizes[1]];
	size_t count = fread(matrix, sizeof(double), sizes[0] * sizes[1], _file);
	assert( count == sizes[0] * sizes[1] );
	fclose(_file);

	int counter = 0;
	std::generate(first, last, [&] { return matrix[counter++]; } );

	delete[] matrix;
}

template<class ForwardIt>
void verify_results(ForwardIt first, ForwardIt last)
{
	uint32_t m = MatrixMul::TEST_C_SIZE[0], l = MatrixMul::TEST_C_SIZE[1];
	double * C_exp = new double[m*l];
	initialize(C_exp, C_exp + m*l, MatrixMul::TEST_C, MatrixMul::TEST_C_SIZE);

	uint32_t i = 0, j = 0;
	for(;i < m;++i) {
		for(j = 0;j < l;++j) {
			uint32_t index = i*l + j;
			if( fabs(*first - C_exp[index]) > DOUBLE_EPS ) {

				printf("Position: (%d,%d), expected %f, got %f\n", i, j, C_exp[index], *first);
				assert( false );
			}
			++first;

			if(first == last) {
				break;
			}
		}
	}

	if(i != m || first != last) {

		std::cout << "Size of computed matrix different than expected!" << std::endl;
		delete[] C_exp;
		assert( false );

	}

	delete[] C_exp;
}

void get_matrix_sizes(const MatrixMulArgs & cur_args, uint32_t & m, uint32_t & k, uint32_t & l)
{
	if( cur_args.test ) {
		m = MatrixMul::TEST_A_SIZE[0];
		k = MatrixMul::TEST_A_SIZE[1];
		l = MatrixMul::TEST_B_SIZE[1];
	} else {
		m = cur_args.matrix_size;
		k = cur_args.matrix_size;
		l = cur_args.matrix_size;
	}
}

template<class ForwardIt, class Generator>
void initialize_matrices(ForwardIt firstA, ForwardIt lastA, ForwardIt firstB, ForwardIt lastB, Generator & gen, const MatrixMulArgs & args)
{
	if( args.test ) {

		initialize(firstA, lastA, MatrixMul::TEST_A, MatrixMul::TEST_A_SIZE);
		initialize(firstB, lastB, MatrixMul::TEST_B, MatrixMul::TEST_B_SIZE);

	} else {

		initialize(firstA, lastA, gen);
		initialize(firstB, lastB, gen);

	}
}

milliseconds MatrixMul::mult_blas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: BLAS ";
	const MatrixMulArgs & cur_args = dynamic_cast<const MatrixMulArgs&>(args);

	uint32_t m, k, l;
	get_matrix_sizes(cur_args, m, k, l);

	double * A = new double[m * k];
	double * B = new double[k * l];
	double * C = new double[m * l];

	initialize_matrices(A, A + m*k, B, B + k*l, gen, cur_args);

	auto start = std::chrono::high_resolution_clock::now();
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, l, k,
				1.0, A, k, B, l, 0.0, C, l);
	auto end = std::chrono::high_resolution_clock::now();

	if( args.test ) {
		verify_results(C, C + m*l);
	}

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;

	delete[] A;
	delete[] B;
	delete[] C;

	return time;
}

milliseconds MatrixMul::mult_blaze(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: Blaze ";
	const MatrixMulArgs & cur_args = dynamic_cast<const MatrixMulArgs&>(args);
	uint32_t m, k, l;
	get_matrix_sizes(cur_args, m, k, l);

	blaze::DynamicMatrix<double, blaze::rowMajor> A(m, k), B(k, l), C(m, l);
	initialize_matrices(A.data(), A.data() + m*k, B.data(), B.data() + k*l, gen, cur_args);

	auto start = std::chrono::high_resolution_clock::now();
	C = A * B;
	auto end = std::chrono::high_resolution_clock::now();

	if( args.test ) {
		verify_results(C.data(), C.data() + m*l);
	}

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

milliseconds MatrixMul::boost_ublas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: boost uBLAS ";
	const MatrixMulArgs & cur_args = dynamic_cast<const MatrixMulArgs&>(args);
	boost::numeric::ublas::matrix<double> A(cur_args.matrix_size, cur_args.matrix_size),
			B(cur_args.matrix_size, cur_args.matrix_size), C(cur_args.matrix_size, cur_args.matrix_size);
	initialize(A.data().begin(), A.data().end(), gen);
	initialize(B.data().begin(), B.data().end(), gen);

	auto start = std::chrono::high_resolution_clock::now();
	noalias(C) = prod( A, B );

	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

milliseconds MatrixMul::plain_call(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: plain call ";

	const MatrixMulArgs & cur_args = dynamic_cast<const MatrixMulArgs&>(args);
	uint32_t elements_count = cur_args.matrix_size*cur_args.matrix_size;
	double * A = new double[elements_count];
	double * B = new double[elements_count];
	double * C = new double[elements_count];

	/**
		Initialize
	**/
	initialize(A, A + elements_count, gen);
	initialize(B, B + elements_count, gen);

	/**
		Compute
	**/
	auto start = std::chrono::high_resolution_clock::now();

	for(uint32_t i = 0; i < cur_args.matrix_size;++i) {

		for(uint32_t k = 0; k < cur_args.matrix_size; ++k) {
			C[i*cur_args.matrix_size + k] = A[i*cur_args.matrix_size] * B[k];
                }

		for(uint32_t j = 1; j < cur_args.matrix_size; ++j) {
			for(uint32_t k = 0; k < cur_args.matrix_size; ++k) {
				C[i*cur_args.matrix_size + k] += A[i*cur_args.matrix_size + j] * B[j*cur_args.matrix_size + k];
			}
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	delete[] A;
	delete[] B;
	delete[] C;

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

milliseconds MatrixMul::blitz(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: blitz++ ";

	const MatrixMulArgs & cur_args = dynamic_cast<const MatrixMulArgs&>(args);
	blitz::Array<double, 2> C(cur_args.matrix_size, cur_args.matrix_size), B(cur_args.matrix_size, cur_args.matrix_size),
			A(cur_args.matrix_size, cur_args.matrix_size);

	initialize(A.begin(), A.end(), gen);
	initialize(B.begin(), B.end(), gen);

	auto start = std::chrono::high_resolution_clock::now();
    blitz::firstIndex i;
    blitz::secondIndex j;
    blitz::thirdIndex k;
    C = blitz::sum(A(i,k) * B(k,j), k);
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}
