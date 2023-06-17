//
// Created by choleraplague on 17.06.23.
//

#include "MatrixFactory.h"
#include "Matrix.h"

Matrix MatrixFactory::GenerateMatrix(unsigned rows, unsigned cols, double minVal, double maxVal) {
	auto matrix = Matrix(rows, cols);
	for(auto i = 0u; i < matrix.Rows(); ++i) {
		for(auto& el : matrix[i]) {
			el = (maxVal - minVal) * uni(rng) + minVal;
		}
	}
	return matrix;
}