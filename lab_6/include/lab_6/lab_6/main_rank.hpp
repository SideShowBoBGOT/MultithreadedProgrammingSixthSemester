#ifndef MAINRANK_HPP
#define MAINRANK_HPP

enum class AlgorithmType;

namespace boost::mpi {
	class communicator;
}

auto main_rank(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned taks_num,
	const AlgorithmType& alg_type
) -> void;

#endif //MAINRANK_HPP
