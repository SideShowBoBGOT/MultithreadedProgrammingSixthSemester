#include <lab_7/child_rank.hpp>
#include <lab_7/common.hpp>
#include <lab_7/alg_type.hpp>
#include <lab_7/constants.hpp>
#include <lab_7/log_error.hpp>

namespace child_rank {

	static auto read_main_handle(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type
	) -> inter::managed_shared_memory::handle_t {
		auto main_handle = inter::managed_shared_memory::handle_t();
		switch(alg_type) {
			case AlgorithmType::OneToMany: {
				boost::mpi::broadcast(world, main_handle, MAIN_PROC_RANK);
				break;
			}
			case AlgorithmType::ManyToOne: {
				for(auto task_rank = 1; task_rank < world.size(); ++task_rank) {
					if(task_rank == world.rank()) {
						auto handles = std::vector<inter::managed_shared_memory::handle_t>();
						boost::mpi::gather(world, main_handle, handles, task_rank);
						main_handle = handles[MAIN_PROC_RANK];
					} else {
						boost::mpi::gather(world, main_handle, task_rank);
					}
				}
				break;
			}
			case AlgorithmType::ManyToMany: {
				auto handles = std::vector<inter::managed_shared_memory::handle_t>();
				boost::mpi::all_gather(world, main_handle, handles);
				main_handle = handles[MAIN_PROC_RANK];
				break;
			}
		}
		return main_handle;
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
			case AlgorithmType::ManyToOne: {
				for(auto task_rank = 0; task_rank < world.size(); ++task_rank) {
					if(task_rank == world.rank()) {
						auto payloads = std::vector<inter::managed_shared_memory::handle_t>();
						boost::mpi::gather(world, handle, payloads, task_rank);
					} else {
						boost::mpi::gather(world, handle, task_rank);
					}
				}
				break;
			}
			case AlgorithmType::ManyToMany: {
				auto handles = std::vector<inter::managed_shared_memory::handle_t>();
				boost::mpi::all_gather(world, handle, handles);
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
