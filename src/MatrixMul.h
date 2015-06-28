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

using std::chrono::milliseconds;

namespace MatrixMul {


	static const std::array<uint32_t,2> TEST_A_SIZE{2000, 1000};
	static const std::string TEST_A = "test/test_data_2000_1000_A";
	static const std::array<uint32_t,2> TEST_B_SIZE{1000, 1500};
	static const std::string TEST_B = "test/test_data_1000_1500_B";
	static const std::array<uint32_t,2> TEST_C_SIZE{2000, 1500};
	static const std::string TEST_C{"test/test_data_2000_1500_C"};

	milliseconds mult_blas(const Args & args, std::mt19937 & gen);

	milliseconds mult_blaze(const Args & args, std::mt19937 & gen);

	milliseconds plain_call(const Args & args, std::mt19937 & gen);

	milliseconds boost_ublas(const Args & args, std::mt19937 & gen);

	milliseconds blitz(const Args & args, std::mt19937 & gen);
}



#endif /* SRC_MATRIXMUL_H_ */
