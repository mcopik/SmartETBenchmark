/*
 * MatrixMul.h
 *
 *  Created on: Jun 28, 2015
 *      Author: mcopik
 */

#ifndef SRC_MATRIXMUL_H_
#define SRC_MATRIXMUL_H_
#include <chrono>
#include <array>

#include "Args.h"

#define DOUBLE_EPS 1e-6

using std::chrono::microseconds;

namespace MatrixMul {

	template<class ForwardIt, class Generator>
	void initialize(ForwardIt first, ForwardIt last, Generator & gen)
	{
		std::uniform_real_distribution<> dis(0, 10);
		std::generate(first, last, [&] { return dis(gen); } );
	}

	template<typename Data>
	class Matrix {
	public:
		uint32_t m, n;
		Data * data;
		Matrix(uint32_t _m, uint32_t _n) : m(_m), n(_n), data(new double[m*n])
		{
		}
		~Matrix()
		{
			delete data;
		}

		Matrix & operator=(const Matrix & a)
		{

			for(uint32_t i = 0; i < m;++i) {
				for(uint32_t j = 0; j < n;++j) {

					data[i*n + j] = a.data[i*n + j];
				}
			}

			return *this;
		}
	};

	template<typename Data>
	Matrix<Data> operator*(const Matrix<Data> & A, const Matrix<Data> & B)
	{
		Matrix<Data> C(A.m, B.n);

		for(uint32_t i = 0; i < A.m;++i) {

			for(uint32_t n = 0; n < A.n; ++n) {
				C.data[i*B.n + n] = A.data[i*A.n] * B.data[n];
			}

			for(uint32_t j = 1; j < A.n; ++j) {
				for(uint32_t n = 0; n < A.n; ++n) {
					C.data[i*B.n + n] += A.data[i*A.n + j] * B.data[j*B.n + n];
				}
			}
		}

		return C;
	}

	static const std::array<uint32_t,2> TEST_A_SIZE{2000, 1000};
	static const std::string TEST_A = "test/test_data_2000_1000_A";
	static const std::array<uint32_t,2> TEST_B_SIZE{1000, 1500};
	static const std::string TEST_B = "test/test_data_1000_1500_B";
	static const std::array<uint32_t,2> TEST_C_SIZE{2000, 1500};
	static const std::string TEST_C{"test/test_data_2000_1500_C"};

	microseconds mult_blas(const Args & args, std::mt19937 & gen);

	microseconds mult_blaze(const Args & args, std::mt19937 & gen);

	microseconds plain_call(const Args & args, std::mt19937 & gen);

	microseconds boost_ublas(const Args & args, std::mt19937 & gen);

	microseconds blitz(const Args & args, std::mt19937 & gen);

	microseconds op_overl(const Args & args, std::mt19937 & gen);
}



#endif /* SRC_MATRIXMUL_H_ */
