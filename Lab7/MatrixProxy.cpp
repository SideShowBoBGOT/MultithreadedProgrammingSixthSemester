#include "MatrixProxy.h"
#include "Matrix.h"

MatrixProxy::MatrixProxy(unsigned fromRow, unsigned toRow, unsigned fromCol, unsigned toCol) :
    fromRow{fromRow}, toRow{toRow}, fromCol{fromCol}, toCol{toCol} {}

MatrixProxy::InnerRow& MatrixProxy::operator[](unsigned index) {
    
	return matrix->innerMat()[index];
}

const MatrixProxy::InnerRow& MatrixProxy::operator[](unsigned index) const {
	return matrix->innerMat()[index];
}

unsigned MatrixProxy::rows() const {
	return ;
}

unsigned MatrixProxy::cols() const {
	return mat.front().size();
}