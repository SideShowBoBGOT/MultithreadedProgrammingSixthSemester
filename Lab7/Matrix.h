#ifndef _MATRIX_H
#define _MATRIX_H

#include "IMatrix.h"

class Matrix : public IMatrix {
	public:
	Matrix()=default;
	Matrix(unsigned rows, unsigned cols);
	virtual ~Matrix() override=default;

	public:
	virtual const InnerMat& innerMat() const;
	virtual InnerMat& innerMat();

	public:
	virtual unsigned rows() const override;
	virtual unsigned cols() const override;
	
	public:
	virtual void sum(const Matrix& other) override;

	public:
	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
	
	virtual InnerRow& operator[](unsigned index) override;
	virtual const InnerRow& operator[](unsigned index) const override;

	private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar & mat;
		}
		
	protected:
	InnerMat mat;
};


#endif //LAB6_MATRIX_H
