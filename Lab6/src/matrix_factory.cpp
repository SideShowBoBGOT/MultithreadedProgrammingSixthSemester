#include <lab_6/matrix_factory.hpp>

auto MatrixFactory::generate(
	const unsigned rows,
	const unsigned cols,
	const double minVal,
	const double maxVal
) -> mpi_mul::Matrix {
	auto matrix = mpi_mul::Matrix(rows, cols);
	for(auto i = 0u; i < rows;++i) {
		for(auto& el : matrix[i]) {
			el = (maxVal - minVal) * uni_(rng_) + minVal;
		}
	}
	return matrix;
}