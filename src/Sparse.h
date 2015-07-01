
#ifndef SRC_SPARSE_H_
#define SRC_SPARSE_H_
#include <chrono>

#include "Args.h"


using std::chrono::microseconds;

namespace Sparse {


	microseconds blaze(const Args & args, std::mt19937 & gen);

	microseconds boost_ublas(const Args & args, std::mt19937 & gen);
}



#endif
