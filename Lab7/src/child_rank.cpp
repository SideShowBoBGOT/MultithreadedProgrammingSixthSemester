#include <lab_6/child_rank.hpp>
#include <lab_6/common.hpp>
#include <lab_6/alg_type.hpp>
#include <lab_6/constants.hpp>

namespace child_rank {

	static auto read_main_handle(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type
	) -> inter::managed_shared_memory::handle_t {
		auto payload = inter::managed_shared_memory::handle_t();
		switch(alg_type) {
			case AlgorithmType::OneToMany: {
				boost::mpi::broadcast(world, payload, FROM_MAIN_THREAD_TAG);
				break;
			}
		}
		return payload;
	}

	static auto send_child_handle(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const inter::managed_shared_memory::handle_t handle
	) -> void {
		switch(alg_type) {
			case AlgorithmType::OneToMany: {
				for(auto task_rank = 1; task_rank < world.size(); ++task_rank) {
					auto handle_copy = handle;
					boost::mpi::broadcast(world, handle_copy, task_rank);
				}
				break;
			}
		}
	}

	auto execute(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned step_length,
		const common::MatSizes& sizes
	) -> void {
		const auto main_handle = read_main_handle(world, alg_type);
		const auto main_shared_memory = inter::managed_shared_memory(inter::open_only, MAIN_SHARED_MEMORY_NAME.data());
		const auto [first, second] = get_main_matrices(main_shared_memory, main_handle, sizes);
		const auto child_handle = calculate_partial_result(world, step_length, sizes, first, second);
		send_child_handle(world, alg_type, child_handle);
	}
}