#ifndef LAB6_MATRIX_H
#define LAB6_MATRIX_H

#include <vector>
#include <iostream>

class Matrix {
	public:
	Matrix()=default;
	Matrix(unsigned rows, unsigned cols);

	public:
	void sum(const Matrix& other);
	Matrix mul(const Matrix& other) const;

	public:
	unsigned Matrix::rows() const;
	unsigned Matrix::cols() const;

	public:
	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
	std::vector<double>& operator[](unsigned index);
	const std::vector<double>& operator[](unsigned index) const;

	protected:
	std::vector<std::vector<double>> mat;
};


#endif //LAB6_MATRIX_H
