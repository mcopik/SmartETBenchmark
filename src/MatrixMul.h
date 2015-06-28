/*
 * MatrixMul.h
 *
 *  Created on: Jun 28, 2015
 *      Author: mcopik
 */

#ifndef SRC_MATRIXMUL_H_
#define SRC_MATRIXMUL_H_
#include <chrono>

#include "Args.h"

using std::chrono::milliseconds;

namespace MatrixMul {

	milliseconds mult_blas(const Args & args, std::mt19937 & gen);

	milliseconds mult_blaze(const Args & args, std::mt19937 & gen);

	milliseconds plain_call(const Args & args, std::mt19937 & gen);

	milliseconds boost_ublas(const Args & args, std::mt19937 & gen);

	milliseconds blitz(const Args & args, std::mt19937 & gen);
}



#endif /* SRC_MATRIXMUL_H_ */
