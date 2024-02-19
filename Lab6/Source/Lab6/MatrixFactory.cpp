#include <Lab6/MatrixFactory.h>

Matrix MatrixFactory::GenerateMatrix(const unsigned rows, const unsigned cols, const double minVal, const double maxVal) {
	auto matrix = Matrix(rows, cols);
	for(auto i = 0u; i < matrix.rows();++i) {
		for(auto& el : matrix[i]) {
			el = (maxVal - minVal) * uni(rng) + minVal;
		}
	}
	return matrix;
}