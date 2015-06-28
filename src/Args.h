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

struct ManyMatrixMulArgs : public Args {
public:
	ManyMatrixMulArgs(uint32_t m_, uint32_t n_, uint32_t k_, uint32_t l_) :
		m(m_), n(n_), k(k_), l(l_) {}
	uint32_t m;
	uint32_t n;
	uint32_t k;
	uint32_t l;
};


#endif /* SRC_ARGS_H_ */
