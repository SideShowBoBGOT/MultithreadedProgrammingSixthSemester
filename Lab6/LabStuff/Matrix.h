//
// Created by choleraplague on 17.06.23.
//

#ifndef LAB6_MATRIX_H
#define LAB6_MATRIX_H

#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

class Matrix {
	public:
	Matrix()=default;
	Matrix(unsigned rows, unsigned cols);
	virtual ~Matrix()=default;

	public:
	virtual std::vector<double>& operator[](unsigned index);
	virtual unsigned Rows();
	virtual unsigned Cols();

	private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar & mat;
		}
		
	protected:
	using InnerMat = std::vector<std::vector<double>>;
	InnerMat mat;
};


#endif //LAB6_MATRIX_H
