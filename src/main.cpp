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
#include "NestedExpr.h"
#include "Rearrangement.h"
#include "Sparse.h"

typedef std::chrono::microseconds (*matrix_func)(const Args &, std::mt19937 &);

int process_options(CMDOptions * opts, int argc, char ** argv)
{


    struct option long_options[] =
      {
        /* These options set a flag. */
        {"test", no_argument, (int*) &opts->test, 1},
        {"verbose", no_argument, (int*) &opts->verbose, 1},
        {"trashing", no_argument, (int*) &opts->trashing, 1},
        /* These options don’t set a flag.
           We distinguish them by their indices. */
        {"matrix_size", required_argument, 0, 'm'},
        {"repeat_count", required_argument, 0, 'r'},
        {0, 0, 0, 0}
      };

	/**
	 * Example of getopt usage:
	 * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
	 */
	int c;
    int option_index = 0;
	while ( (c = getopt_long (argc, argv, "b:m:r", long_options, &option_index) ) != -1) {
		switch (c)
		{
			case 0:
			break;
			case 'b':
				opts->benchmark_case = atoi(optarg);
			break;
			case 'm':
				opts->matrix_size = atoi(optarg);
			break;
			case 'r':
				opts->repeat_count = atoi(optarg);
			break;

			default:
				printf("%c\n", c);
				return 1;
		}
	}
	return 0;
}

int main(int argc, char ** argv)
{
	CMDOptions options{
			.test = false,
			.verbose = false,
			.trashing = false,
			.benchmark_case = 0,
			.matrix_size = 1000,
			.repeat_count = 10
	};

	if( process_options(&options, argc, argv) ) {
		abort();
		return 1;
	}

	if( options.test ) {
		options.repeat_count = 1;
	}

	std::string benchmark_names[] = {
			"Matrix Multiplication",
			"Nested Expressions",
			"Rearrangement",
			"Sparse"
	};

	std::vector< std::vector<matrix_func> > benchmark_functions{

		{&MatrixMul::op_overl, &MatrixMul::boost_ublas, &MatrixMul::plain_call, &MatrixMul::mult_blaze, &MatrixMul::mult_blas, &MatrixMul::blitz},

		{&NestedExpr::mult_blas, &NestedExpr::mult_blaze, &NestedExpr::plain_call, &NestedExpr::blitz, &NestedExpr::boost_ublas },

		{&Rearrangement::mult_blas, &Rearrangement::mult_blaze, &Rearrangement::mult_blaze_explicit, &Rearrangement::boost_ublas},

		{&Sparse::blaze, &Sparse::boost_ublas}

	};

	std::vector< std::shared_ptr<Args> > benchmark_args {

		std::shared_ptr<Args>(new MatrixMulArgs{options.matrix_size, options.test}),
		std::shared_ptr<Args>(new NestedExprArgs{options.matrix_size, options.test}),
		std::shared_ptr<Args>(new RearrangementArgs{options.matrix_size, options.matrix_size, options.test}),
		std::shared_ptr<Args>(new SparseArgs{options.matrix_size, options.test})

	};
	std::random_device rd;
	std::mt19937 gen(rd());

	auto benchmark = benchmark_functions[options.benchmark_case];
	auto benchmark_arg = benchmark_args[options.benchmark_case];

	size_t size = benchmark.size();
	std::vector<int64_t> timings;
	std::cout << "Benchmark: " << benchmark_names[options.benchmark_case] << std::endl;

	for(size_t i = 0; i < size; ++i) {
		for(uint32_t j = 0; j < options.repeat_count; ++j) {
			timings.push_back( benchmark[i]( *( benchmark_arg.get() ), gen).count() );
		}

		double mean = std::accumulate(timings.begin(), timings.end(), 0.0);
		mean /= options.repeat_count;
		std::nth_element(timings.begin(), std::next(timings.begin(), options.repeat_count/2), timings.end());
		double median =  timings[options.repeat_count/2];
		std::cout << "Avg: " << mean << " Median: " << median << std::endl;
		timings.clear();
	}

	return 0;
}




