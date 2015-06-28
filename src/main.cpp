/*
 * main.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: mcopik
 */
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <map>
#include <set>
#include <iostream>
#include <memory>

/**
 * GNU C library.
 */
#include <getopt.h>

#include "Args.h"
#include "MatrixMul.h"

typedef std::chrono::milliseconds (*matrix_func)(const Args &, std::mt19937 &);

int main(int argc, char ** argv)
{

	const size_t repeat_count = 10;
	uint32_t matrix_size = 1000;
	if( argc > 1 ) {
		matrix_size = atoi( argv[1] );
	}


	std::map<std::string, std::vector<matrix_func> > benchmark_functions{

		std::make_pair< std::string, std::vector<matrix_func> >("Matrix Multiplication",
				{&MatrixMul::boost_ublas, &MatrixMul::plain_call, &MatrixMul::mult_blaze, &MatrixMul::mult_blas, &MatrixMul::blitz}),

	};
	std::map<std::string, std::shared_ptr<Args>> benchmark_args {

		std::make_pair("Matrix Multiplication", std::shared_ptr<Args>(new MatrixMulArgs{matrix_size}) ),

	};
	std::random_device rd;
	std::mt19937 gen(rd());

	auto benchmark = benchmark_functions.begin();
	auto benchmark_arg = benchmark_args.begin();

	while( benchmark != benchmark_functions.end() ) {

		size_t size = (*benchmark).second.size();
		std::vector<int64_t> timings;
		std::cout << "Benchmark: " << (*benchmark).first << std::endl;

		for(size_t i = 0; i < size; ++i) {
			for(uint32_t j = 0; j < repeat_count; ++j) {
				timings.push_back( (*benchmark).second[i]( *( (*benchmark_arg).second.get() ), gen).count() );
			}

			double mean = std::accumulate(timings.begin(), timings.end(), 0.0);
			mean /= repeat_count;
			std::nth_element(timings.begin(), std::next(timings.begin(), repeat_count), timings.end());
			double median =  timings[repeat_count/2];
			std::cout << "Avg: " << mean << " Median: " << median << std::endl;
			timings.clear();
		}

		++benchmark, ++benchmark_arg;
	}

	return 0;
}




