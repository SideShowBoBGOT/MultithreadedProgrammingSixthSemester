#ifndef LAB6_MATRIXFACTORY_H
#define LAB6_MATRIXFACTORY_H

#include <Lab6/Matrix.h>
#include <random>

class MatrixFactory {
	public:
	MatrixFactory()=default;

	public:
	Matrix GenerateMatrix(unsigned rows, unsigned cols,
		double minVal, double maxVal);
	
	protected:
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	std::uniform_real_distribution<double> uni =
		std::uniform_real_distribution<double>(0, 1);
};


#endif //LAB6_MATRIXFACTORY_H
