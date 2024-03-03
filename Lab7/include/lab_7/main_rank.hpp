#ifndef MAIN_RANK_HPP
#define MAIN_RANK_HPP

#include <chrono>
#include <boost/mpi.hpp>

namespace common {
	struct MatSizes;
}

enum class AlgorithmType;

namespace main_rank {
	struct AlgStatistic {
		int world_size = 0;
		std::chrono::system_clock::duration mpi_dur;
		std::chrono::system_clock::duration single_dur;
	};

	auto execute(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned step_length,
		const common::MatSizes& sizes
	) -> AlgStatistic;
}

#endif //MAIN_RANK_HPP
