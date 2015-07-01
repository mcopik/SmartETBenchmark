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

#include "Rearrangement.h"

template<class ForwardIt, class Generator>
void initialize(ForwardIt first, ForwardIt last, Generator & gen)
{
	std::uniform_real_distribution<> dis(0, 10);
	std::generate(first, last, [&] { return dis(gen); } );
}

microseconds Rearrangement::mult_blas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: BLAS ";
	const RearrangementArgs & cur_args = dynamic_cast<const RearrangementArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t v = cur_args.vector_size;
	uint32_t matr_size = m * v;

	double * A = new double[matr_size];
	double * B = new double[matr_size];
	double * x = new double[v];
	double * y = new double[v];
	double * tmp = new double[v];

	initialize(A, A + matr_size, gen);
	initialize(B, B + matr_size, gen);
	initialize(x, x + v, gen);

	auto start = std::chrono::high_resolution_clock::now();
	cblas_dgemv(CblasRowMajor, CblasNoTrans, m, v, 1.0, B, v, x, 1, 0.0, tmp, 1);
	cblas_dgemv(CblasRowMajor, CblasNoTrans, m, v, 1.0, A, v, tmp, 1, 0.0, y, 1);
	auto end = std::chrono::high_resolution_clock::now();


	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;

	delete[] A;
	delete[] B;
	delete[] x;
	delete[] y;
	delete[] tmp;

	return time;
}

microseconds Rearrangement::mult_blaze(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: Blaze ";
	const RearrangementArgs & cur_args = dynamic_cast<const RearrangementArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t v = cur_args.vector_size;
	uint32_t matr_size = m * v;

	blaze::DynamicMatrix<double, blaze::rowMajor> A(m, v), B(m, v);
	blaze::DynamicVector<double> x(v), y(v);

	initialize(A.data(), A.data() + matr_size, gen);
	initialize(B.data(), B.data() + matr_size, gen);
	initialize(x.data(), x.data() + v, gen);

	auto start = std::chrono::high_resolution_clock::now();
	y = A * B * x;
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

microseconds Rearrangement::mult_blaze_explicit(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: Blaze explicit ";
	const RearrangementArgs & cur_args = dynamic_cast<const RearrangementArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t v = cur_args.vector_size;
	uint32_t matr_size = m * v;

	blaze::DynamicMatrix<double, blaze::rowMajor> A(m, v), B(m, v);
	blaze::DynamicVector<double> x(v), y(v);

	initialize(A.data(), A.data() + matr_size, gen);
	initialize(B.data(), B.data() + matr_size, gen);
	initialize(x.data(), x.data() + v, gen);

	auto start = std::chrono::high_resolution_clock::now();
	y = eval(A * B) * x;
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

microseconds Rearrangement::boost_ublas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: boost uBLAS ";
	const RearrangementArgs & cur_args = dynamic_cast<const RearrangementArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t v = cur_args.vector_size;
	uint32_t matr_size = m * v;

	boost::numeric::ublas::matrix<double> A(m, v), B(m, v);
	boost::numeric::ublas::vector<double> x(v), y(v);

	initialize(A.data().begin(), A.data().end(), gen);
	initialize(B.data().begin(), B.data().end(), gen);
	initialize(x.data().begin(), x.data().end(), gen);

	auto start = std::chrono::high_resolution_clock::now();
	noalias(y) = prod( prod(A, B), x );
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;

	return time;
}

