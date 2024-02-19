#ifndef LAB6_MATRIX_H
#define LAB6_MATRIX_H

#include <vector>
#include <boost/serialization/vector.hpp>

class Matrix {
	public:
	Matrix()=default;
	Matrix(unsigned rows, unsigned cols);

	public:
	using InnerMat = std::vector<std::vector<double>>;

	public:
	const InnerMat& innerMat() const;
	unsigned rows() const;
	unsigned cols() const;
	
	public:
	virtual void sum(const Matrix& other);

	public:
	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
	std::vector<double>& operator[](unsigned index);
	const std::vector<double>& operator[](unsigned index) const;

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
