//
// Created by choleraplague on 17.06.23.
//

#ifndef LAB6_IMATRIXALGORITHM_H
#define LAB6_IMATRIXALGORITHM_H

#include <vector>

class IMatrixAlgorithm {
	public:
	IMatrixAlgorithm()=default;
	virtual ~IMatrixAlgorithm()=default;
	
	public:
	using Matrix = std::vector<std::vector<double>>;
	
	public:
	virtual Matrix Multiply(unsigned threadsNum, const Matrix& first, const Matrix& second)=0;
};

#endif //LAB6_IMATRIXALGORITHM_H
