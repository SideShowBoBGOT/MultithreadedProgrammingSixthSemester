#ifndef LAB6_MATRIXFACTORY_H
#define LAB6_MATRIXFACTORY_H

#include <vector>
#include <random>

class Matrix;

class MatrixFactory {
	public:
	MatrixFactory()=default;
	virtual ~MatrixFactory()=default;
	
	public:
	virtual Matrix GenerateMatrix(unsigned rows, unsigned cols,
								  double minVal, double maxVal);
	
	protected:
	std::random_device rd;
	std::mt19937 rng = std::mt19937(rd());
	std::uniform_real_distribution<double> uni =
		std::uniform_real_distribution<double>(0, 1);
};


#endif //LAB6_MATRIXFACTORY_H
