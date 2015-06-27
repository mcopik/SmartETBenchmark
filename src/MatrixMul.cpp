#include <random>
#include <iostream>
#include <set>
#include <cstddef>

#include <boost/numeric/ublas/matrix.hpp>

#include <blaze/math/DynamicMatrix.h>

#include <mkl_cblas.h>

#include "MatrixMul.h"

template<class ForwardIt, class Generator>
void initialize(ForwardIt first, ForwardIt last, Generator & gen)
{
	std::uniform_real_distribution<> dis(0, 10);
	std::generate(first, last, [&] { return dis(gen); } );
}

milliseconds MatrixMul::mult_blas(uint32_t size, std::mt19937 & gen)
{
	std::cout << "Test: BLAS ";
	uint32_t elements_count = size*size;
	double * A = new double[elements_count];
	double * B = new double[elements_count];
	double * C = new double[elements_count];

	initialize(A, A + elements_count, gen);
	initialize(B, B + elements_count, gen);

	auto start = std::chrono::high_resolution_clock::now();
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, size, size, size,
				1.0, A, size, B, size, 0.0, C, size);
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

milliseconds MatrixMul::mult_blaze(uint32_t size, std::mt19937 & gen)
{
	std::cout << "Test: Blaze ";
	blaze::DynamicMatrix<double, blaze::rowMajor> A(size, size), B(size, size), C(size,size);
	initialize(A.data(), A.data() + size*size, gen);
	initialize(B.data(), B.data() + size*size, gen);
	auto start = std::chrono::high_resolution_clock::now();
	C = A * B;
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

milliseconds MatrixMul::boost_ublas(uint32_t size, std::mt19937 & gen)
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

milliseconds MatrixMul::plain_call(uint32_t size, std::mt19937 & gen)
{
	std::cout << "Test: plain call ";

	uint32_t elements_count = size*size;
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

milliseconds MatrixMul::blitz(uint32_t size, std::mt19937 & gen)
{
	std::cout << "Test: blitz++ ";

	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}
