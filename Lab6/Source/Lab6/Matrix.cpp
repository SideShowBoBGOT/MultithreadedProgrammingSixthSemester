#include <Lab6/Matrix.h>
#include <iostream>

static constexpr char OPEN_BRACKETS_SYMBOL = '{';
static constexpr char CLOSING_BRACKETS_SYMBOL = '}';
static constexpr char SPACE_SYMBOL = ' ';

static const std::string MATRICES_NOT_THE_SAME_SIZE = "Matrices are not the same size";

Matrix::Matrix(const unsigned int rows, const unsigned int cols) {
	mat.resize(rows);
	for(auto& row : mat) {
		row.assign(cols, 0);
	}
}

std::vector<double>& Matrix::operator[](const unsigned index) {
	return mat[index];
}

const std::vector<double>& Matrix::operator[](const unsigned index) const {
	return mat[index];
}

const Matrix::InnerMat& Matrix::innerMat() const {
	return mat;
}

unsigned Matrix::rows() const {
	return mat.size();
}

unsigned Matrix::cols() const {
	return mat.front().size();
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
	out << OPEN_BRACKETS_SYMBOL << SPACE_SYMBOL;
	for(auto i = 0u; i < matrix.rows(); ++i) {
		out << OPEN_BRACKETS_SYMBOL << SPACE_SYMBOL;
		for(auto j = 0u; j < matrix.cols(); ++j) {
			out << matrix[i][j] << SPACE_SYMBOL;
		}
		out << CLOSING_BRACKETS_SYMBOL << SPACE_SYMBOL;
	}
	out << CLOSING_BRACKETS_SYMBOL << SPACE_SYMBOL;
	return out;
}

void Matrix::sum(const Matrix& other) {
	if(rows() != other.rows() || cols() != other.cols()) {
		throw std::invalid_argument(MATRICES_NOT_THE_SAME_SIZE);
	}
	
	const auto& otherInnerMat = other.innerMat();
	for(auto i = 0u; i < rows(); ++i){
		for(auto j = 0u; j < cols(); ++j) {
			mat[i][j] += otherInnerMat[i][j];
		}
	}
}
