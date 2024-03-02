#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <span>
#include <vector>
#include <iostream>

template<typename Container>
struct RectContainer {
	std::vector<Container> Data;
	friend std::ostream& operator<<(std::ostream& out, const RectContainer& obj) {
		out << "{ ";
		for(const auto& row : obj.Data) {
			out << "{ ";
			for(const auto& el : row) {
				out << el << " ";
			}
			out << "} ";
		}
		out << "}";
		return out;
	}
};

using RectSpan = RectContainer<std::span<double>>;
using RectVector = RectContainer<std::vector<double>>;

class MatrixSpan {
	public:
	MatrixSpan(const unsigned rows, const unsigned cols, double* ptr);

	public:
	unsigned get_rows() const;
	unsigned get_cols() const;
	std::span<double> get_at(const unsigned row) const;
	double get_at(const unsigned row, const unsigned col) const;
	RectVector mul(const MatrixSpan& other) const;
	void randomize();
	friend std::ostream& operator<<(std::ostream& out, const MatrixSpan& mat);

	protected:
	std::span<double> data;
	unsigned rows = 0;
	unsigned cols = 0;
};

#endif //MATRIX_HPP