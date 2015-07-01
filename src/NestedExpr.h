
#ifndef SRC_NESTEDEXPR_H_
#define SRC_NESTEDEXPR_H_
#include <chrono>
#include <array>

#include "Args.h"

#define DOUBLE_EPS 1e-6

using std::chrono::microseconds;

namespace NestedExpr {

	microseconds mult_blas(const Args & args, std::mt19937 & gen);

	microseconds mult_blaze(const Args & args, std::mt19937 & gen);

	microseconds plain_call(const Args & args, std::mt19937 & gen);

	microseconds boost_ublas(const Args & args, std::mt19937 & gen);

	microseconds blitz(const Args & args, std::mt19937 & gen);
}



#endif
