#include <lab_6/cli_app.hpp>
#include <lab_6/matrix.hpp>
#include <lab_6/constants.hpp>
#include <lab_6/log_error.hpp>

#include <boost/mpi.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <magic_enum.hpp>

#include <random>
#include <string_view>
#include <limits>
#include <filesystem>
#include <thread>

namespace inter = boost::interprocess;

static const auto MAPPED_ALG_TYPES = [] {
	constexpr auto values = magic_enum::enum_values<AlgorithmType>();
	constexpr auto names = magic_enum::enum_names<AlgorithmType>();
	auto mapped = std::unordered_map<std::string, AlgorithmType>();
	for(auto i = 0u; i < values.size(); ++i) {
		mapped.insert_or_assign(names[i].data(), values[i]);
	}
	return mapped;
}();

static const std::string MAIN_SHARED_MEMORY_NAME = "eeeeeeeeeeeeeiomopnohjubuybnijsdbgiuarsbifbaeuigbeslgnuiesgnieangloersiugn";
static const std::string CHILD_SHARED_MEMORY_NAME = "fdogsdniubu43769223ndsosdfijspofmakanapmk_";

// PROBABLY SYSTEM DEPENDANT
static constexpr auto MINIMUM_ALLOC_SIZE = 224 * 2;

static auto get_steps(
	const unsigned child_rank,
	const unsigned first_rows,
	const unsigned step_length
) -> unsigned {
	const auto len = static_cast<double>(first_rows - child_rank + 1);
	return static_cast<unsigned>(std::ceil(len / step_length));
}

static auto get_child_memory_name(
	const unsigned child_rank
) -> std::string {
	return CHILD_SHARED_MEMORY_NAME + std::to_string(child_rank);
}

struct MatSizes {
	unsigned first_rows = 0;
	unsigned first_cols = 0;
	unsigned second_rows = 0;
	unsigned second_cols = 0;
};

static auto get_main_matrices(
	const inter::managed_shared_memory& shm,
	const long handle,
	const MatSizes& sizes
) -> std::tuple<MatrixSpan, MatrixSpan> {
	const auto first_mat_space = sizeof(double) * sizes.first_rows * sizes.first_cols;
	const auto ptr = static_cast<char*>(shm.get_address_from_handle(handle));
	auto first_mat = MatrixSpan(sizes.first_rows, sizes.first_cols, reinterpret_cast<double*>(ptr));
	auto second_mat = MatrixSpan(sizes.second_rows, sizes.second_cols, reinterpret_cast<double*>(ptr + first_mat_space));
	return std::make_tuple(first_mat, second_mat);
}

struct AlgStatistic {
	int world_size = 0;
	std::chrono::system_clock::duration mpi_dur;
	std::chrono::system_clock::duration single_dur;
};

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

	struct SingleStats {
		std::chrono::system_clock::duration duration;
		RectVector result;
	};

	static auto multiply_single(
		const MatrixSpan& first,
		const MatrixSpan& second
	) -> SingleStats {
		const auto start_time = std::chrono::system_clock::now();
		auto result = first.mul(second);
		const auto end_time = std::chrono::system_clock::now();
		const auto duration = end_time - start_time;
		return {duration, std::move(result)};
	}

	struct InitStats {
		SingleStats single_stats;
		inter::managed_shared_memory::handle_t handle = 0;
	};

	static auto init_matrices_and_single_stats(
		const MatSizes& sizes
	) -> InitStats {
		const auto first_mat_space = sizeof(double) * sizes.first_rows * sizes.first_cols;
		const auto second_mat_space = sizeof(double) * sizes.second_rows * sizes.second_cols;
		const auto alloc_size = first_mat_space + second_mat_space;
		try {
			auto shared_memory = inter::managed_shared_memory(inter::create_only, MAIN_SHARED_MEMORY_NAME.data(),
				alloc_size + MINIMUM_ALLOC_SIZE, nullptr);
			if(const auto free_size = shared_memory.get_free_memory(); alloc_size > free_size) {
				ERROR(std::format("Free size {} is less than alloc size {}", free_size, alloc_size));
			}
			const auto alloc_ptr = shared_memory.allocate(alloc_size);
			const auto handle = shared_memory.get_handle_from_address(alloc_ptr);
			auto [first_mat, second_mat] = get_main_matrices(shared_memory, handle, sizes);
			first_mat.randomize();
			second_mat.randomize();
			return {multiply_single(first_mat, second_mat), handle};
		} catch(const boost::interprocess::interprocess_exception& ex) {
			ERROR(ex.what());
		}
	}

	static auto send_info(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned tasks_num,
		const inter::managed_shared_memory::handle_t handle
	) -> void {
		switch(alg_type) {
			case AlgorithmType::Blocking: {
				for(auto i = 0; i < tasks_num; ++i) {
					world.send(i + 1, FROM_MAIN_THREAD_TAG, handle);
				}
				break;
			}
			case AlgorithmType::NonBlocking: {
				std::vector<boost::mpi::request> sent;
				for(auto i = 0; i < tasks_num; ++i) {
					sent.push_back(world.isend(i + 1, FROM_MAIN_THREAD_TAG, handle));
				}
				boost::mpi::wait_all(sent.begin(), sent.end());
				break;
			}
		}
	}

	static auto collect_results(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned tasks_num,
		const unsigned step_length,
		const unsigned first_rows,
		const unsigned second_cols
	) -> std::tuple<
		std::vector<inter::managed_shared_memory>,
		std::vector<SharedMemoryRemover>,
		RectSpan
	> {
		auto child_memories = std::vector<inter::managed_shared_memory>();
		auto child_removers = std::vector<SharedMemoryRemover>();
		child_memories.reserve(tasks_num);
		child_removers.reserve(tasks_num);
		auto result = std::vector<std::span<double>>(first_rows);
		for(auto child_rank = 1u; child_rank <= tasks_num; ++child_rank) {
			long child_handle = 0;
			switch(alg_type) {
				case AlgorithmType::Blocking: {
					world.recv(static_cast<int>(child_rank), FROM_TASK_THREAD_TAG, child_handle);
					break;
				}
				case AlgorithmType::NonBlocking: {
					world.irecv(static_cast<int>(child_rank), FROM_TASK_THREAD_TAG, child_handle).wait();
					break;
				}
			}
			const auto steps = get_steps(child_rank, first_rows, step_length);
			auto memory_name = get_child_memory_name(child_rank);
			auto child_memory = inter::managed_shared_memory(inter::open_only, memory_name.data());
			child_removers.emplace_back(std::move(memory_name));
			const auto ptr = child_memory.get_address_from_handle(child_handle);
			const auto partial_result = MatrixSpan(steps, second_cols, static_cast<double*>(ptr));
			const auto initial_index = child_rank - 1;
			for(auto i = 0u; i < steps; ++i) {
				result[initial_index + i * step_length] = partial_result.get_at(i);
			}
			child_memories.emplace_back(std::move(child_memory));
		}
		return std::make_tuple(std::move(child_memories), std::move(child_removers), RectSpan{std::move(result)});
	}

	static auto check_results(
		const RectVector& single_res,
		const RectSpan& mpi_res
	) -> void {
		// LOG_INFO("Single: " << single_res);
		// LOG_INFO("MPI: " << mpi_res);
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
		const unsigned tasks_num,
		const MatSizes& sizes
	) -> AlgStatistic {
		const auto guard = main_rank::MainMemoryGuard();
		const auto [single_stats, handle] = main_rank::init_matrices_and_single_stats(sizes);
		const auto mpi_start_time = std::chrono::system_clock::now();

		main_rank::send_info(world, alg_type, tasks_num, handle);
		const auto [child_memories, child_memory_removers, mpi_result] = main_rank::collect_results(
			world, alg_type, tasks_num, step_length, sizes.first_rows, sizes.second_cols);
		// LOG_INFO("RECEIVED RESULTS");
		const auto mpi_dur = std::chrono::system_clock::now() - mpi_start_time;
		main_rank::check_results(single_stats.result, mpi_result);
		return AlgStatistic{world.size(), mpi_dur, single_stats.duration};
	}
}

namespace child_rank {

	static auto read_payload(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type
	) -> inter::managed_shared_memory::handle_t {
		auto payload = inter::managed_shared_memory::handle_t();
		switch(alg_type) {
			case AlgorithmType::Blocking: {
				world.recv(0, FROM_MAIN_THREAD_TAG, payload);
				break;
			}
			case AlgorithmType::NonBlocking: {
				auto received = std::vector {
					world.irecv(0, FROM_MAIN_THREAD_TAG, payload),
				};
				boost::mpi::wait_all(received.begin(), received.end());
				break;
			}
		}
		return payload;
	}

	static auto notify_main(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const long handle
	) -> void {
		switch(alg_type) {
			case AlgorithmType::Blocking: {
				world.send(0, FROM_TASK_THREAD_TAG, handle);
				break;
			}
			case AlgorithmType::NonBlocking: {
				world.isend(0, FROM_TASK_THREAD_TAG, handle).wait();
				break;
			}
		}
	}

	static auto execute(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned step_length,
		const MatSizes& sizes
	) -> void {
		const auto main_handle = read_payload(world, alg_type);
		const auto main_shared_memory = inter::managed_shared_memory(inter::open_only, MAIN_SHARED_MEMORY_NAME.data());
		const auto [first, second] = get_main_matrices(main_shared_memory, main_handle, sizes);
		const auto rank = static_cast<unsigned>(world.rank());
		const auto steps = get_steps(rank, sizes.first_rows, step_length);
		const auto alloc_memory = sizeof(double) * steps * sizes.second_cols;
		const auto memory_name = get_child_memory_name(rank);
		inter::shared_memory_object::remove(memory_name.data());
		auto child_memory = inter::managed_shared_memory(inter::create_only, memory_name.data(), alloc_memory + MINIMUM_ALLOC_SIZE, nullptr);
		const auto ptr = child_memory.allocate(alloc_memory);
		const auto child_handle = child_memory.get_handle_from_address(ptr);
		const auto child_result = MatrixSpan(steps, sizes.second_cols, static_cast<double*>(ptr));
		for(auto row_index = rank - 1, i = 0u;
			i < steps; row_index += step_length, ++i) {
			// LOG_INFO("STEP: " << i);
			auto row = child_result.get_at(i);
			for(auto j = 0; j < sizes.second_cols; ++j) {
				auto value = 0.0;
				for(auto k = 0; k < sizes.first_cols; ++k) {
					value += first.get_at(row_index, k) * second.get_at(k, j);
				}
				row[j] = value;
			}
		}
		notify_main(world, alg_type, child_handle);
	}

}



auto main_logic(
	const MatSizes& sizes,
	const AlgorithmType& alg_type
) -> std::optional<AlgStatistic> {

	if(sizes.first_cols != sizes.second_rows) {
		ERROR("Impossible to multiply");
	}

	const auto env = boost::mpi::environment();
	const auto world = boost::mpi::communicator();

	const auto subprocs_num = [&world] {
		const auto subprocs_num = world.size() - 1;
		if(subprocs_num <= 0) {
			ERROR("Number of tasks is less or equal zero");
		}
		return static_cast<unsigned>(subprocs_num);
	}();

	const auto [tasks_num, step_length] = [&sizes, &subprocs_num] {
		const auto is_rows_less = sizes.first_rows < subprocs_num;
		const auto tasks_num = is_rows_less ? sizes.first_rows : subprocs_num;
		const auto step_length = is_rows_less ? 1 : tasks_num;
		return std::make_tuple(tasks_num, step_length);
	}();

	const auto rank = world.rank();
	if(rank == 0) {
		return main_rank::execute(world, alg_type, step_length, tasks_num, sizes);
	}
	if(rank <= tasks_num) {
		child_rank::execute(world, alg_type, step_length, sizes);
	}
	return std::nullopt;
}

CliApp::CliApp() : CLI::App("lab_6") {
	add_option("--size,-s", size, "Matrix size")
		->required(true)
		->check(CLI::PositiveNumber);
	add_option("--type,-t", alg_type, "Algorithm type")
		->transform(CLI::CheckedTransformer(MAPPED_ALG_TYPES, CLI::ignore_case))
		->required(true);

	parse_complete_callback([this] {
		const auto res = main_logic({size, size, size, size}, alg_type);
		if(res) {
			const auto& statistic = res.value();
			const auto nano_mpi = static_cast<double>(statistic.mpi_dur.count());
			const auto nano_single = static_cast<double>(statistic.single_dur.count());
			const auto efficiency = nano_single / nano_mpi;
			auto log_file = std::ofstream("log.txt", std::ios::app);
			if(not log_file.is_open()) {
				ERROR("Error opening log file");
			}
			log_file << std::format("{}\t{}\t{}\t{}\t{}\t{}\n", statistic.world_size, size,
			magic_enum::enum_name(alg_type), nano_mpi, nano_single, efficiency);
		}
	});
}
