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

#include "NestedExpr.h"

template<class ForwardIt, class Generator>
void initialize(ForwardIt first, ForwardIt last, Generator & gen)
{
	std::uniform_real_distribution<> dis(0, 10);
	std::generate(first, last, [&] { return dis(gen); } );
}

microseconds NestedExpr::mult_blas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: BLAS ";
	const NestedExprArgs & cur_args = dynamic_cast<const NestedExprArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t matr_size = m * m;

	double * A = new double[matr_size];
	double * B = new double[matr_size];
	double * C = new double[matr_size];
	double * D = new double[matr_size];
	double * E = new double[matr_size];

	initialize(A, A + matr_size, gen);
	initialize(B, B + matr_size, gen);
	initialize(C, C + matr_size, gen);
	initialize(D, D + matr_size, gen);

	auto start = std::chrono::high_resolution_clock::now();
	cblas_daxpy(matr_size, 1.0, A, 1.0, B, 1.0);
	cblas_daxpy(matr_size, -1.0, D, 1.0, C, 1.0);
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m,
				1.0, B, m, C, m, 0.0, E, m);
	auto end = std::chrono::high_resolution_clock::now();


	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;

	delete[] A;
	delete[] B;
	delete[] C;
	delete[] D;
	delete[] E;

	return time;
}

microseconds NestedExpr::mult_blaze(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: Blaze ";
	const NestedExprArgs & cur_args = dynamic_cast<const NestedExprArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t matr_size = m * m;

	blaze::DynamicMatrix<double, blaze::rowMajor> A(m, m), B(m, m), C(m, m), D(m, m), E(m,m);

	initialize(A.data(), A.data() + matr_size, gen);
	initialize(B.data(), B.data() + matr_size, gen);
	initialize(C.data(), C.data() + matr_size, gen);
	initialize(D.data(), D.data() + matr_size, gen);

	auto start = std::chrono::high_resolution_clock::now();
	E = (A - B) * (C - D);
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

microseconds NestedExpr::boost_ublas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: boost uBLAS ";
	const NestedExprArgs & cur_args = dynamic_cast<const NestedExprArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t matr_size = m * m;

	boost::numeric::ublas::matrix<double> A(m, m), B(m, m), C(m, m), D(m, m), E(m,m);

	initialize(A.data().begin(), A.data().end(), gen);
	initialize(B.data().begin(), B.data().end(), gen);
	initialize(C.data().begin(), C.data().end(), gen);
	initialize(D.data().begin(), D.data().end(), gen);

	auto start = std::chrono::high_resolution_clock::now();
	noalias(E) = prod( A + B, C - D );
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;

	return time;
}

microseconds NestedExpr::plain_call(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: plain call nested ";

	const NestedExprArgs & cur_args = dynamic_cast<const NestedExprArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t matr_size = m * m;

	double * A = new double[matr_size];
	double * B = new double[matr_size];
	double * C = new double[matr_size];
	double * D = new double[matr_size];
	double * E = new double[matr_size];

	initialize(A, A + matr_size, gen);
	initialize(B, B + matr_size, gen);
	initialize(C, C + matr_size, gen);
	initialize(D, D + matr_size, gen);



	/**
		Compute
	**/
	auto start = std::chrono::high_resolution_clock::now();

	for(uint32_t i = 0; i < m;++i) {

		for(uint32_t j = 0; j < m; ++j) {

			A[i*m + j] += B[i*m + j];
			C[i*m + j] -= D[i*m + j];

		}
	}

	for(uint32_t i = 0; i < m;++i) {

		for(uint32_t j = 0; j < m; ++j) {
			E[i*m + j] = A[i*m] * C[j];
		}

		for(uint32_t j = 1; j < m; ++j) {
			for(uint32_t k = 0; k < m; ++k) {
				E[i*m + k] += A[i*m + j] * C[j*m + k];
			}
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	delete[] A;
	delete[] B;
	delete[] C;
	delete[] D;
	delete[] E;


	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

microseconds NestedExpr::blitz(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: blitz++ ";

	const NestedExprArgs & cur_args = dynamic_cast<const NestedExprArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t matr_size = m * m;

	blitz::Array<double, 2> A(m, m), B(m, m), C(m, m), D(m, m), E(m,m);

	auto start = std::chrono::high_resolution_clock::now();
    blitz::firstIndex i;
    blitz::secondIndex j;
    blitz::thirdIndex n;
    A += B;
    C -= D;
    E = blitz::sum(A(i,n) * C(n,j), n);
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}
