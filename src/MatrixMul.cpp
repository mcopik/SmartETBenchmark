#include <random>
#include <iostream>
#include <chrono>
#include <cstddef>

#include <boost/numeric/ublas/matrix.hpp>

template<class ForwardIt, class Generator>
void initialize(ForwardIt first, ForwardIt last, Generator & gen)
{
	std::uniform_real_distribution<> dis(0, 10);
	std::generate(first, last, [&] { return dis(gen); } );
}

std::chrono::milliseconds boost_ublas(uint32_t size)
{
	std::cout << "Test: boost uBLAS\n";

	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
}

std::chrono::milliseconds plain_call(uint32_t size)
{
	std::cout << "Test: plain call\n";

	uint32_t elements_count = size*size;
	double * A = new double[elements_count];
	double * B = new double[elements_count];
	double * C = new double[elements_count];

	/**
		Initialize
	**/
	std::random_device rd;
        std::mt19937 gen(rd());
	initialize(A, A + elements_count, gen);
	initialize(B, B + elements_count, gen);
	initialize(C, C + elements_count, gen);

	/**
		Compute
	**/
	auto start = std::chrono::high_resolution_clock::now();

	for(uint32_t i = 0; i < size;++i) {

		for(uint32_t k = 0; k < size; ++k) {
			C[i*size + k] = A[i*size] * B[k];
                }

		for(uint32_t j = 1; j < size; ++j) {
			for(uint32_t k = 0; k < size; ++k) {
				C[i*size + k] += A[i*size + j] * B[j*size + k];
			}
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	delete[] A;
	delete[] B;
	delete[] C;

	return std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
}

typedef std::chrono::milliseconds (*matrix_func)(uint32_t);

int main(int argc, char ** argv)
{
	uint32_t matrix_size = 1000;
	if( argc > 1 ) {
		matrix_size = atoi( argv[1] );
	}

	matrix_func functions[]{&boost_ublas, &plain_call};
	size_t size = sizeof(functions)/sizeof(matrix_func);

	for(size_t i = 0; i < size; ++i) {
		std::cout << "Finished in: " << (*functions[i])(matrix_size).count() << " ms" << std::endl;
	}


	return 0;
}
