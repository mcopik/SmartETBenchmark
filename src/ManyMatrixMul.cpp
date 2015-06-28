#include <random>
#include <iostream>
#include <set>
#include <cstddef>

#include <boost/numeric/ublas/matrix.hpp>

#include <blaze/math/DynamicMatrix.h>

#include <mkl_cblas.h>

#include "ManyMatrixMul.h"

template<class ForwardIt, class Generator>
void initialize(ForwardIt first, ForwardIt last, Generator & gen)
{
	std::uniform_real_distribution<> dis(0, 10);
	std::generate(first, last, [&] { return dis(gen); } );
}

milliseconds ManyMatrixMul::mult_blas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: BLAS ";
	const ManyMatrixMulArgs & cur_args = dynamic_cast<const ManyMatrixMulArgs&>(args);
	double * A = new double[cur_args.m * cur_args.n];
	double * B = new double[cur_args.n * cur_args.k];
	double * C = new double[cur_args.k * cur_args.l];
	double * D = new double[cur_args.m * cur_args.l];
	double * AB = new double[cur_args.m * cur_args.n];

	initialize(A, A + cur_args.m * cur_args.n, gen);
	initialize(B, B + cur_args.n * cur_args.k, gen);
	initialize(C, C + cur_args.k * cur_args.l, gen);

	auto start = std::chrono::high_resolution_clock::now();
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, cur_args.m, cur_args.k, cur_args.n,
				1.0, A, cur_args.n, B, cur_args.k, 0.0,  AB, cur_args.k);
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, cur_args.m, cur_args.l, cur_args.k,
				1.0, AB, cur_args.k, C, cur_args.l, 0.0,  D, cur_args.l);
	auto end = std::chrono::high_resolution_clock::now();


	delete[] A;
	delete[] B;
	delete[] C;
	delete[] D;
	delete[] AB;

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

milliseconds ManyMatrixMul::mult_blaze(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: Blaze ";
	const ManyMatrixMulArgs & cur_args = dynamic_cast<const ManyMatrixMulArgs&>(args);
	blaze::DynamicMatrix<double, blaze::rowMajor> A(cur_args.m, cur_args.n), B(cur_args.n, cur_args.k),
			C(cur_args.k, cur_args.l), D(cur_args.m, cur_args.l);
	initialize(A.data(), A.data() + cur_args.m * cur_args.n, gen);
	initialize(B.data(), B.data() + cur_args.n * cur_args.k, gen);
	initialize(C.data(), C.data() + cur_args.k * cur_args.l, gen);
	auto start = std::chrono::high_resolution_clock::now();
	D = A * B * C;
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}
/*
milliseconds ManyMatrixMul::boost_ublas(uint32_t size, std::mt19937 & gen)
{
	std::cout << "Test: boost uBLAS ";
	boost::numeric::ublas::matrix<double> A(size, size), B(size, size), C(size, size);
	initialize(A.data().begin(), A.data().end(), gen);
	initialize(B.data().begin(), B.data().end(), gen);

	auto start = std::chrono::high_resolution_clock::now();
	noalias(C) = prod( A, B );

	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

milliseconds ManyMatrixMul::plain_call(uint32_t size, std::mt19937 & gen)
{
	std::cout << "Test: plain call ";

	uint32_t elements_count = size*size;
	double * A = new double[elements_count];
	double * B = new double[elements_count];
	double * C = new double[elements_count];

	/**
		Initialize
	**/
	//initialize(A, A + elements_count, gen);
	//initialize(B, B + elements_count, gen);

	/**
		Compute
	**/
	/*auto start = std::chrono::high_resolution_clock::now();

	for(uint32_t i = 0; i < size;++i) {

		for(uint32_t k = 0; k < size; ++k) {
			C[i*size + k] = A[i*size] * B[k];
                }

		for(uint32_t j = 1; j < size; ++j) {
			for(uint32_t k = 0; k < size; ++k) {
				C[i*size + k] += A[i*size + j] * B[j*size + k];
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

milliseconds ManyMatrixMul::blitz(uint32_t size, std::mt19937 & gen)
{
	std::cout << "Test: blitz++ ";

	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}
*/
