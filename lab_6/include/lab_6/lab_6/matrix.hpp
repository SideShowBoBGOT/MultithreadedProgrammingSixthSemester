#ifndef LAB6_MATRIX_H
#define LAB6_MATRIX_H

#include <boost/serialization/vector.hpp>

#include <vector>
#include <iostream>

class Matrix {
	public:
	Matrix();
	Matrix(unsigned rows, unsigned cols);

	public:
	unsigned get_rows() const;
	unsigned get_cols() const;

	public:
	Matrix& operator+=(const Matrix& other);
	Matrix operator*(const Matrix& other) const;
	bool operator==(const Matrix& other) const;
	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

	public:
	std::vector<double>& operator[](unsigned index);
	const std::vector<double>& operator[](unsigned index) const;

	private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
		ar & mat;
	}

	protected:
	std::vector<std::vector<double>> mat;
};


#endif //LAB6_MATRIX_H
