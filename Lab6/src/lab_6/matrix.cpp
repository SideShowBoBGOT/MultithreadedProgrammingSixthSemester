#include <lab_6/matrix.hpp>
#include <lab_6/log_error.hpp>

#include <iostream>

Matrix::Matrix()=default;

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

unsigned Matrix::get_rows() const {
	return mat.size();
}

unsigned Matrix::get_cols() const {
	return mat.empty() ? 0 : mat.front().size();
}

Matrix& Matrix::operator+=(const Matrix& other) {
	if(get_rows() != other.get_rows() || get_cols() != other.get_cols()) {
		ERROR("Matrices are not the same size");
	}
	for(auto i = 0u; i < get_rows(); ++i){
		for(auto j = 0u; j < get_cols(); ++j) {
			mat[i][j] += other.mat[i][j];
		}
	}
	return *this;
}

Matrix Matrix::operator*(const Matrix& other) const {
	if(not get_cols() == other.get_rows()) {
		ERROR("Number of columns of the first matrix is not"
		 "the same as number of rows of the second");
	}

	auto result = Matrix(get_rows(), get_cols());

	for(auto i = 0; i < get_rows(); ++i) {
		for(auto j = 0; j < other.get_cols(); ++j) {
			auto value = 0.0;
			for(auto k = 0; k < get_cols(); ++k) {
				value += mat[i][k] * other.mat[k][j];
			}
			result.mat[i][j] = value;
		}
	}

	return result;
}

bool Matrix::operator==(const Matrix& other) const {
	return mat == other.mat;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
	out << "{" << " ";
	for(auto i = 0u; i < matrix.get_rows(); ++i) {
		out << "{" << " ";
		for(auto j = 0u; j < matrix.get_cols(); ++j) {
			out << matrix[i][j] << " ";
		}
		out << "}" << " ";
	}
	out << "}" << " ";
	return out;
}