#include <lab_7/main_rank.hpp>
#include <lab_7/matrix.hpp>
#include <lab_7/common.hpp>
#include <lab_7/constants.hpp>
#include <lab_7/log_error.hpp>
#include <lab_7/alg_type.hpp>
#include <lab_7/mat_sizes.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>

namespace main_rank {

	struct MainMemoryGuard {
		MainMemoryGuard() {
			inter::shared_memory_object::remove(MAIN_SHARED_MEMORY_NAME.data());
		}
		~MainMemoryGuard() {
			inter::shared_memory_object::remove(MAIN_SHARED_MEMORY_NAME.data());
		}
	};

	struct SharedMemoryRemover {
		SharedMemoryRemover()=default;
		SharedMemoryRemover(std::string&& _name) : name{std::move(_name)} {}
		~SharedMemoryRemover() {
			inter::shared_memory_object::remove(name.data());
		}
		protected:
		std::string name;
	};

	static auto multiply_single(
		const mmat::MatrixSpan& first,
		const mmat::MatrixSpan& second
	) -> std::tuple<
			std::chrono::system_clock::duration,
			mmat::RectVector
		> {
		const auto start_time = std::chrono::system_clock::now();
		auto result = first.mul(second);
		const auto end_time = std::chrono::system_clock::now();
		const auto duration = end_time - start_time;
		return std::make_tuple(duration, std::move(result));
	}

	static auto create_matrices(
		const common::MatSizes& sizes
	) -> std::tuple<
		inter::managed_shared_memory,
		inter::managed_shared_memory::handle_t,
		mmat::MatrixSpan,
		mmat::MatrixSpan
	> {
		const auto first_mat_space = sizeof(double) * sizes.first_rows * sizes.first_cols;
		const auto second_mat_space = sizeof(double) * sizes.second_rows * sizes.second_cols;
		const auto alloc_size = first_mat_space + second_mat_space;
		auto shared_memory = inter::managed_shared_memory(inter::create_only, MAIN_SHARED_MEMORY_NAME.data(),
				alloc_size + MINIMUM_ALLOC_SIZE, nullptr);
		if(const auto free_size = shared_memory.get_free_memory(); alloc_size > free_size) {
			ERROR(std::format("Free size {} is less than alloc size {}", free_size, alloc_size));
		}
		const auto alloc_ptr = shared_memory.allocate(alloc_size);
		const auto main_handle = shared_memory.get_handle_from_address(alloc_ptr);
		auto [first_mat, second_mat] = get_main_matrices(shared_memory, main_handle, sizes);
		first_mat.randomize();
		second_mat.randomize();
		return std::make_tuple(std::move(shared_memory), main_handle, first_mat, second_mat);
	}

	static auto send_main_handle(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const inter::managed_shared_memory::handle_t handle
	) -> void {
		switch(alg_type) {
			case AlgorithmType::OneToMany: {
				auto handle_copy = handle;
				boost::mpi::broadcast(world, handle_copy, MAIN_PROC_RANK);
				break;
			}
			case AlgorithmType::ManyToOne: {
				for(auto task_rank = 1; task_rank < world.size(); ++task_rank) {
					boost::mpi::gather(world, handle, task_rank);
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

	static auto add_partial_result(
		const unsigned first_rows,
		const unsigned second_cols,
		const unsigned step_length,
		const unsigned task_rank,
		const inter::managed_shared_memory::handle_t task_handle,
		std::vector<inter::managed_shared_memory>& tasks_memories,
		std::vector<SharedMemoryRemover>& tasks_removers,
		std::vector<std::span<double>>& result
	) -> void {

		const auto steps = common::get_steps(task_rank, first_rows, step_length);
		auto memory_name = common::get_task_memory_name(task_rank);
		auto child_memory = inter::managed_shared_memory(inter::open_only, memory_name.data());
		tasks_removers.emplace_back(std::move(memory_name));
		const auto ptr = child_memory.get_address_from_handle(task_handle);
		const auto partial_result = mmat::MatrixSpan(steps, second_cols, static_cast<double*>(ptr));
		const auto initial_index = task_rank;
		for(auto i = 0u; i < steps; ++i) {
			result[initial_index + i * step_length] = partial_result.get_at(i);
		}
		tasks_memories.emplace_back(std::move(child_memory));
	}

	static auto collect_results(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned step_length,
		const unsigned first_rows,
		const unsigned second_cols,
		const inter::managed_shared_memory::handle_t main_task_handle
	) -> std::tuple<
			std::vector<inter::managed_shared_memory>,
			std::vector<SharedMemoryRemover>,
			mmat::RectSpan
		> {
		const auto tasks_num = world.size();
		auto tasks_memories = std::vector<inter::managed_shared_memory>();
		auto tasks_removers = std::vector<SharedMemoryRemover>();
		tasks_memories.reserve(tasks_num);
		tasks_removers.reserve(tasks_num);
		auto result = std::vector<std::span<double>>(first_rows);

		add_partial_result(first_rows, second_cols, step_length, MAIN_PROC_RANK, main_task_handle, tasks_memories, tasks_removers, result);

		switch(alg_type) {
			case AlgorithmType::OneToMany: {
				for(auto task_rank = 1; task_rank < tasks_num; ++task_rank) {
					auto child_handle = inter::managed_shared_memory::handle_t();
					boost::mpi::broadcast(world, child_handle, task_rank);
					add_partial_result(first_rows, second_cols, step_length, task_rank, child_handle, tasks_memories, tasks_removers, result);
				}
				break;
			}
			case AlgorithmType::ManyToOne: {
				auto handles = std::vector<inter::managed_shared_memory::handle_t>();
				boost::mpi::gather(world, main_task_handle, handles, MAIN_PROC_RANK);
				for(auto task_rank = 1; task_rank < tasks_num; ++task_rank) {
					boost::mpi::gather(world, main_task_handle, task_rank);
					add_partial_result(first_rows, second_cols, step_length, task_rank, handles[task_rank], tasks_memories, tasks_removers, result);
				}
				break;
			}
			case AlgorithmType::ManyToMany: {
				auto handles = std::vector<inter::managed_shared_memory::handle_t>();
				boost::mpi::all_gather(world, main_task_handle, handles);
				for(auto task_rank = 1; task_rank < tasks_num; ++task_rank) {
					add_partial_result(first_rows, second_cols, step_length, task_rank, handles[task_rank], tasks_memories, tasks_removers, result);
				}
				break;
			}
		}
		return std::make_tuple(std::move(tasks_memories), std::move(tasks_removers), mmat::RectSpan{std::move(result)});
	}

	static auto check_results(
		const mmat::RectVector& single_res,
		const mmat::RectSpan& mpi_res
	) -> void {
		if(single_res.Data.size() != mpi_res.Data.size()) {
			ERROR("Rows size does not match");
		}
		for(auto row = 0u; row < single_res.Data.size(); ++row) {
			const auto& row_single = single_res.Data[row];
			const auto& row_mpi = mpi_res.Data[row];
			if(row_single.size() != row_mpi.size()) {
				ERROR("Cols size does not match");
			}
			for(auto col = 0u; col < row_single.size(); ++col) {
				if(std::abs(row_single[col] - row_mpi[col]) > std::numeric_limits<double>::epsilon()) {
					ERROR("Elements does not match");
				}
			}
		}
	}

	auto execute(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned step_length,
		const common::MatSizes& sizes
	) -> AlgStatistic {

		const auto guard = MainMemoryGuard();

		const auto [main_memory, main_handle, first_mat, second_mat] = create_matrices(sizes);
		const auto [single_duration, single_result] = multiply_single(first_mat, second_mat);

		const auto mpi_start_time = std::chrono::system_clock::now();

		send_main_handle(world, alg_type, main_handle);
		const auto task_handle = calculate_partial_result(world, step_length, sizes, first_mat, second_mat);
		const auto [child_memories, child_memory_removers, mpi_result] = collect_results(
			world, alg_type,  step_length, sizes.first_rows, sizes.second_cols, task_handle);

		const auto mpi_dur = std::chrono::system_clock::now() - mpi_start_time;
		check_results(single_result, mpi_result);
		return AlgStatistic{world.size(), mpi_dur, single_duration};
	}

}