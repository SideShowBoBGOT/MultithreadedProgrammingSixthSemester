#include <lab_6/matrix.hpp>
#include <iostream>

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

unsigned Matrix::rows() const {
	return mat.size();
}

unsigned Matrix::cols() const {
	return mat.front().size();
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
	out << "{" << " ";
	for(auto i = 0u; i < matrix.rows(); ++i) {
		out << "{" << " ";
		for(auto j = 0u; j < matrix.cols(); ++j) {
			out << matrix[i][j] << " ";
		}
		out << "}" << " ";
	}
	out << "}" << " ";
	return out;
}

void Matrix::sum(const Matrix& other) {
	if(rows() != other.rows() || cols() != other.cols()) {
		throw std::invalid_argument(MATRICES_NOT_THE_SAME_SIZE);
	}
	
	for(auto i = 0u; i < rows(); ++i){
		for(auto j = 0u; j < cols(); ++j) {
			mat[i][j] += other.mat[i][j];
		}
	}
}

Matrix Matrix::mul(const Matrix& other) const {
	if(not cols() == other.rows()) {
		throw std::logic_error("Cols and rows are not equal");
	}

	auto result = Matrix(rows(), cols());

	for(auto i = 0; i < rows(); ++i) {
		for(auto j = 0; j < other.cols(); ++j) {
			auto value = 0.0;
			for(auto k = 0; k < cols(); ++k) {
				value += mat[i][k] * other.mat[k][j];
			}
			result.mat[i][j] = value;
		}
	}

	return result;
}
