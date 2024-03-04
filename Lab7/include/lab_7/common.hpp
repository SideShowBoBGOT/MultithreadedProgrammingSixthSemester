#ifndef COMMON_HPP
#define COMMON_HPP

#include <lab_7/matrix.hpp>

#include <boost/mpi.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

namespace inter = boost::interprocess;

namespace common {

	struct MatSizes;

	auto get_steps(
		const unsigned rank,
		const unsigned first_rows,
		const unsigned step_length
	) -> unsigned;

	auto get_task_memory_name(
		const unsigned child_rank
	) -> std::string;

	auto get_main_matrices(
		const inter::managed_shared_memory& main_memory,
		const inter::managed_shared_memory::handle_t handle,
		const MatSizes& sizes
	) -> std::tuple<mmat::MatrixSpan, mmat::MatrixSpan>;

	auto calculate_partial_result(
		const boost::mpi::communicator& world,
		const unsigned step_length,
		const MatSizes& sizes,
		const mmat::MatrixSpan& first,
		const mmat::MatrixSpan& second
	) -> inter::managed_shared_memory::handle_t;

}

#endif //COMMON_HPP
