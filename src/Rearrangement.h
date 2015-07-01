
#ifndef SRC_REARRANGEMENT_H_
#define SRC_REARRANGEMENT_H_
#include <chrono>

#include "Args.h"

#define DOUBLE_EPS 1e-6

using std::chrono::microseconds;

namespace Rearrangement {

	microseconds mult_blas(const Args & args, std::mt19937 & gen);

	microseconds mult_blaze(const Args & args, std::mt19937 & gen);

	microseconds mult_blaze_explicit(const Args & args, std::mt19937 & gen);


	microseconds boost_ublas(const Args & args, std::mt19937 & gen);

}



#endif
