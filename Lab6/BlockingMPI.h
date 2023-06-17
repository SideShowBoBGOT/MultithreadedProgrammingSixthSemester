//
// Created by choleraplague on 17.06.23.
//

#ifndef LAB6_BLOCKINGMPI_H
#define LAB6_BLOCKINGMPI_H

#include "IMatrixAlgorithm.h"

class BlockingMPI : public IMatrixAlgorithm {
	public:
	BlockingMPI()=default;
	virtual ~BlockingMPI()=default;
	
	public:
	virtual Matrix Multiply(unsigned threadsNum, const Matrix& first, const Matrix& second) override;
};


#endif //LAB6_BLOCKINGMPI_H
