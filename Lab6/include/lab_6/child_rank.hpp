#ifndef CHILD_RANK_HPP
#define CHILD_RANK_HPP

#include <boost/mpi.hpp>

namespace common {
	struct MatSizes;
}

enum class AlgorithmType;

namespace child_rank {

	auto execute(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned step_length,
		const common::MatSizes& sizes
	) -> void;

}

#endif //CHILD_RANK_HPP
