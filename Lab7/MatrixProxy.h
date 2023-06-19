#ifndef _MATRIX_PROXY_H_
#define _MATRIX_PROXY_H_

#include "IMatrix.h"

class Matrix;

class MatrixProxy : public IMatrix {
    public:
    MatrixProxy()=default;
    virtual ~MatrixProxy()=default;

    public:
    virtual unsigned rows() const override;
	virtual unsigned cols() const override;

    public:
    virtual void sum(const IMatrix& other) override;

    public:
    virtual InnerRow& operator[](unsigned index) override;
	virtual const InnerRow& operator[](unsigned index) const override;

    private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
        ar & matrix;
        ar & fromRow;
        ar & toRow;
        ar & fromCol;
        ar & toCol;
    }

    protected:
    unsigned fromRow = 0;
    unsigned toRow = 0;
    unsigned fromCol = 0;
    unsigned toCol = 0;

    protected:
    Matrix* matrix = nullptr;
}

#endif // _MATRIX_PROXY_H_