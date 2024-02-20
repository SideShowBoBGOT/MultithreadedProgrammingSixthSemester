#ifndef MAINRANK_HPP
#define MAINRANK_HPP

namespace boost::mpi {
	class communicator;
}

auto main_rank(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned taks_num,
	const bool is_blocking
) -> void;

#endif //MAINRANK_HPP
