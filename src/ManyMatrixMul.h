/*
 * ManyMatrixMul.h
 *
 *  Created on: Jun 28, 2015
 *      Author: mcopik
 */

#ifndef SRC_MANYMATRIXMUL_H_
#define SRC_MANYMATRIXMUL_H_
#include <chrono>

#include "Args.h"

using std::chrono::milliseconds;

namespace ManyMatrixMul {

	milliseconds mult_blas(const Args &, std::mt19937 & gen);

	milliseconds mult_blaze(const Args &,std::mt19937 & gen);

	milliseconds plain_call(const Args &,std::mt19937 & gen);

	milliseconds boost_ublas(const Args &,std::mt19937 & gen);

	milliseconds blitz(const Args &, std::mt19937 & gen);
}



#endif /* SRC_MATRIXMUL_H_ */
