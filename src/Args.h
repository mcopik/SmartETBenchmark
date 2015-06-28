/*
 * Args.h
 *
 *  Created on: Jun 28, 2015
 *      Author: mcopik
 */

#ifndef SRC_ARGS_H_
#define SRC_ARGS_H_

struct Args {
	virtual ~Args() {}
};

struct MatrixMulArgs : public Args {
public:
	MatrixMulArgs(uint32_t matrix_size_) : matrix_size(matrix_size_) {}
	uint32_t matrix_size;
};


#endif /* SRC_ARGS_H_ */
