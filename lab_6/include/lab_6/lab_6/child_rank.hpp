#ifndef CHILD_RANK_HPP
#define CHILD_RANK_HPP

enum class AlgorithmType;

namespace boost::mpi {
	class communicator;
}

auto child_rank(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned tasks_num,
	const AlgorithmType& alg_type
) -> void;

#endif //CHILD_RANK_HPP
