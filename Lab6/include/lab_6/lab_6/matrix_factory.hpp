#ifndef LAB6_MATRIXFACTORY_H
#define LAB6_MATRIXFACTORY_H

#include <lab_6/matrix.hpp>
#include <random>

class MatrixFactory {
	public:
	MatrixFactory()=default;

	public:
	Matrix generate(unsigned rows, unsigned cols,
		double minVal, double maxVal);
	
	protected:
	std::random_device rd_;
	std::mt19937 rng_ = std::mt19937(rd_());
	std::uniform_real_distribution<double> uni_ =
		std::uniform_real_distribution<double>(0, 1);
};


#endif //LAB6_MATRIXFACTORY_H
