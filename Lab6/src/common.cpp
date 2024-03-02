#include <lab_6/common.hpp>
#include <lab_6/constants.hpp>
#include <lab_6/mat_sizes.hpp>

namespace common {

	// This is just random string, not a special token
	static const std::string TASK_SHARED_MEMORY_NAME = "fdogsdniubu43769223ndsosdfijspofmakanapmk_";

	auto get_steps(
		const unsigned rank,
		const unsigned first_rows,
		const unsigned step_length
	) -> unsigned {
		const auto len = static_cast<double>(first_rows - rank);
		return static_cast<unsigned>(std::ceil(len / step_length));
	}

	auto get_task_memory_name(
		const unsigned child_rank
	) -> std::string {
		return TASK_SHARED_MEMORY_NAME + std::to_string(child_rank);
	}

	auto get_main_matrices(
		const inter::managed_shared_memory& main_memory,
		const inter::managed_shared_memory::handle_t handle,
		const MatSizes& sizes
	) -> std::tuple<mmat::MatrixSpan, mmat::MatrixSpan> {

		const auto first_mat_space = sizeof(double) * sizes.first_rows * sizes.first_cols;
		const auto ptr = static_cast<char*>(main_memory.get_address_from_handle(handle));
		auto first_mat = mmat::MatrixSpan(sizes.first_rows, sizes.first_cols, reinterpret_cast<double*>(ptr));
		auto second_mat = mmat::MatrixSpan(sizes.second_rows, sizes.second_cols, reinterpret_cast<double*>(ptr + first_mat_space));
		return std::make_tuple(first_mat, second_mat);
	}

	auto calculate_partial_result(
		const boost::mpi::communicator& world,
		const unsigned step_length,
		const MatSizes& sizes,
		const mmat::MatrixSpan& first,
		const mmat::MatrixSpan& second
	) -> inter::managed_shared_memory::handle_t {

		const auto rank = static_cast<unsigned>(world.rank());
		const auto steps = get_steps(rank, sizes.first_rows, step_length);
		const auto alloc_memory = sizeof(double) * steps * sizes.second_cols;
		const auto memory_name = get_task_memory_name(rank);
		inter::shared_memory_object::remove(memory_name.data());
		auto child_memory = inter::managed_shared_memory(inter::create_only,
			memory_name.data(), alloc_memory + MINIMUM_ALLOC_SIZE, nullptr);
		const auto ptr = child_memory.allocate(alloc_memory);
		const auto child_handle = child_memory.get_handle_from_address(ptr);
		const auto child_result = mmat::MatrixSpan(steps, sizes.second_cols, static_cast<double*>(ptr));
		for(auto row_index = rank, i = 0u;
			i < steps; row_index += step_length, ++i) {
			auto row = child_result.get_at(i);
			for(auto j = 0; j < sizes.second_cols; ++j) {
				auto value = 0.0;
				for(auto k = 0; k < sizes.first_cols; ++k) {
					value += first.get_at(row_index, k) * second.get_at(k, j);
				}
				row[j] = value;
			}
		}
		return child_handle;
	}
}