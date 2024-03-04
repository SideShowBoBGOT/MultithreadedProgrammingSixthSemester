#include <lab_7/matrix.hpp>
#include <random>

namespace mmat {
	MatrixSpan::MatrixSpan(const unsigned rows, const unsigned cols, double* ptr) {
		this->rows = rows;
		this->cols = cols;
		data = std::span(ptr, rows * cols);
	}

	unsigned MatrixSpan::get_rows() const {
		return rows;
	}

	unsigned MatrixSpan::get_cols() const {
		return cols;
	}

	std::span<double> MatrixSpan::get_at(const unsigned row) const {
		return data.subspan(row * cols, cols);
	}

	void MatrixSpan::randomize() {
		auto rd_ = std::random_device();;
		auto rng_ = std::mt19937(rd_());
		auto uni_ = std::uniform_real_distribution<double>(0, 1);
		for(auto& value : data) {
			value = uni_(rng_);
		}
	}

	std::ostream& operator<<(std::ostream& out, const MatrixSpan& mat) {
		out << "{ ";
		for(auto i = 0u; i < mat.rows; ++i) {
			const auto row = mat.get_at(i);
			out << "{ ";
			for(auto j = 0u; j < mat.cols; ++j) {
				out << row[j] << " ";
			}
			out << "} ";
		}
		out << "}";
		return out;
	}

	double MatrixSpan::get_at(const unsigned row, const unsigned col) const {
		return data[row * cols + col];
	}

	RectVector MatrixSpan::mul(const MatrixSpan& other) const {
		auto result = std::vector<std::vector<double>>(rows);
		for(auto i = 0; i < rows; ++i) {
			auto row = std::vector<double>(other.cols);
			for(auto j = 0; j < other.cols; ++j) {
				for(auto k = 0; k < cols; ++k) {
					row[j] += this->get_at(i, k) * other.get_at(k, j);
				}
			}
			result[i] = std::move(row);
		}
		return {result};
	}
}