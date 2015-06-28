/*
 * Args.h
 *
 *  Created on: Jun 28, 2015
 *      Author: mcopik
 */

#ifndef SRC_ARGS_H_
#define SRC_ARGS_H_



struct CMDOptions {
	bool test;
	bool verbose;
	bool trashing;
	uint32_t benchmark_case;
	uint32_t matrix_size;
	uint32_t repeat_count;
};

struct Args {
	Args(bool test_) : test(test_) {}
	virtual ~Args() {}
	bool test;
};

struct MatrixMulArgs : public Args {
	MatrixMulArgs(uint32_t matrix_size_, bool test_) : Args(test_), matrix_size(matrix_size_){}
	uint32_t matrix_size;

};


#endif /* SRC_ARGS_H_ */
