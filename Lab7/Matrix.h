#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

class Matrix {
	public:
	Matrix()=default;
	Matrix(unsigned rows, unsigned cols);
	virtual ~Matrix()=default;

	public:
	using InnerMat = std::vector<std::vector<double>>;
	using InnerRow = std::vector<double>;

	public:
	virtual const InnerMat& innerMat() const;
	virtual InnerMat& innerMat();

	public:
	virtual unsigned rows() const;
	virtual unsigned cols() const;
	
	public:
	virtual void sum(const Matrix& other);

	public:
	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
	
	virtual InnerRow& operator[](unsigned index);
	virtual const InnerRow& operator[](unsigned index) const;

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
