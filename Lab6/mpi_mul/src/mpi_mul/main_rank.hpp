#ifndef MAINRANK_HPP
#define MAINRANK_HPP

#include <mpi_mul/matrix.hpp>

namespace boost::mpi {
	class communicator;
}

namespace mpi_mul {
	enum class AlgorithmType;

	auto main_rank(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned tasks_num,
		const Matrix& first,
		const Matrix& second
	) -> Matrix;
}

#endif //MAINRANK_HPP
