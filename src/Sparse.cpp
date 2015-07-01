#include <random>
#include <iostream>
#include <set>
#include <cstddef>
#include <cstdio>
#include <cmath>
#include <stdexcept>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#include <blaze/math/DynamicMatrix.h>
#include <blaze/math/CompressedMatrix.h>

#include <mkl_cblas.h>

#include <blitz/array.h>

#include "Sparse.h"

template<class ForwardIt, class Generator>
void initialize(ForwardIt first, ForwardIt last, Generator & gen)
{
	std::uniform_real_distribution<> dis(0, 10);
	std::generate(first, last, [&] { return dis(gen); } );
}

microseconds Sparse::blaze(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: Blaze ";
	const SparseArgs & cur_args = dynamic_cast<const SparseArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t matr_size = m * m;

	blaze::DynamicMatrix<double, blaze::rowMajor> A(m, m), C(m, m);
	blaze::CompressedMatrix<double> B(m, m);

	initialize(A.data(), A.data() + matr_size, gen);
	uint32_t to_fill = round(matr_size * cur_args.fill);

	std::uniform_int_distribution<> dis(0, m - 1);
	std::uniform_real_distribution<> real_dis(0, 1);
	for(uint32_t i = 0;i < to_fill; ++i) {
		uint32_t pos_x = dis(gen);
		uint32_t pos_y = dis(gen);

		B.set(pos_x, pos_y, real_dis(gen));
	}

	auto start = std::chrono::high_resolution_clock::now();
	C = A * B;
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;
	return time;
}

microseconds Sparse::boost_ublas(const Args & args, std::mt19937 & gen)
{
	std::cout << "Test: boost uBLAS ";
	const SparseArgs & cur_args = dynamic_cast<const SparseArgs&>(args);

	uint32_t m = cur_args.matrix_size;
	uint32_t matr_size = m * m;

	boost::numeric::ublas::matrix<double> A(m, m), C(m, m);
	boost::numeric::ublas::compressed_matrix<double> B(m, m);

	initialize(A.data().begin(), A.data().end(), gen);

	uint32_t to_fill = round(matr_size * cur_args.fill);

	std::uniform_int_distribution<> dis(0, m - 1);
	std::uniform_real_distribution<> real_dis(0, 1);
	for(uint32_t i = 0;i < to_fill; ++i) {
		uint32_t pos_x = dis(gen);
		uint32_t pos_y = dis(gen);

		B.push_back(pos_x, pos_y, real_dis(gen));
	}

	auto start = std::chrono::high_resolution_clock::now();
	noalias(C) = prod( A, B );
	auto end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>( end - start);
	std::cout << time.count() << std::endl;

	return time;
}
