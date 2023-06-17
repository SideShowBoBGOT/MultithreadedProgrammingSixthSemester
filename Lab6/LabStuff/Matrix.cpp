//
// Created by choleraplague on 17.06.23.
//

#include "Matrix.h"

Matrix::Matrix(unsigned int rows, unsigned int cols) {
	mat.resize(rows);
	for(auto& row : mat) {
		row.assign(cols, 0);
	}
}

std::vector<double>& Matrix::operator[](unsigned index) {
	return mat[index];
}

unsigned Matrix::Rows() {
	return mat.size();
}

unsigned Matrix::Cols() {
	return mat.front().size();
}
