#ifndef CHILD_RANK_HPP
#define CHILD_RANK_HPP

enum class AlgorithmType;

namespace boost::mpi {
	class communicator;
}

namespace mpi_mul {
	auto child_rank(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type
	) -> void;
}

#endif //CHILD_RANK_HPP
