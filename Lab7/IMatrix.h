#ifndef _IMATRIX_H_
#define _IMATRIX_H_

#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

class IMatrix {
    public:
    IMatrix()=default;
    virtual ~IMatrix()=default;

    public:
    using InnerMat = std::vector<std::vector<double>>;
    using InnerRow = std::vector<double>;

    public:
    virtual unsigned rows() const=0;
	virtual unsigned cols() const=0;

    public:
    virtual void sum(const IMatrix& other)=0;

    public:
    virtual InnerRow& operator[](unsigned index)=0;
	virtual const InnerRow& operator[](unsigned index) const=0;

    private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {}

    protected:
	static const std::string MATRICES_NOT_THE_SAME_SIZE;
};

const std::string IMatrix::MATRICES_NOT_THE_SAME_SIZE = "Matrices are not the same size";

#endif // _IMATRIX_H_