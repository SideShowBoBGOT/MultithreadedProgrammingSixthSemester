#ifndef CHILD_RANK_HPP
#define CHILD_RANK_HPP

namespace boost::mpi {
	class communicator;
}

auto child_rank(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned tasks_num,
	const bool is_blocking
) -> void;

#endif //CHILD_RANK_HPP
