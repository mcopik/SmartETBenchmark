#include <random>
#include <iostream>
#include <set>
#include <cstddef>

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

milliseconds MatrixMul::mult_blas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: BLAS ";
	const MatrixMulArgs & cur_args = dynamic_cast<const MatrixMulArgs&>(args);
	uint32_t elements_count = cur_args.matrix_size * cur_args.matrix_size;
	double * A = new double[elements_count];
	double * B = new double[elements_count];
	double * C = new double[elements_count];

	initialize(A, A + elements_count, gen);
	initialize(B, B + elements_count, gen);

	auto start = std::chrono::high_resolution_clock::now();
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, cur_args.matrix_size, cur_args.matrix_size, cur_args.matrix_size,
				1.0, A, cur_args.matrix_size, B, cur_args.matrix_size, 0.0, C, cur_args.matrix_size);
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

milliseconds MatrixMul::mult_blaze(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: Blaze ";
	const MatrixMulArgs & cur_args = dynamic_cast<const MatrixMulArgs&>(args);
	blaze::DynamicMatrix<double, blaze::rowMajor> A(cur_args.matrix_size, cur_args.matrix_size),
			B(cur_args.matrix_size, cur_args.matrix_size), C(cur_args.matrix_size,cur_args.matrix_size);
	initialize(A.data(), A.data() + cur_args.matrix_size*cur_args.matrix_size, gen);
	initialize(B.data(), B.data() + cur_args.matrix_size*cur_args.matrix_size, gen);
	auto start = std::chrono::high_resolution_clock::now();
	C = A * B;
	auto end = std::chrono::high_resolution_clock::now();

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
