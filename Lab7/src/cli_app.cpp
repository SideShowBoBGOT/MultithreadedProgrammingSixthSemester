#include <lab_6/cli_app.hpp>
#include <lab_6/log_error.hpp>
#include <lab_6/main_rank.hpp>
#include <lab_6/child_rank.hpp>
#include <lab_6/mat_sizes.hpp>

#include <boost/mpi.hpp>
#include <magic_enum.hpp>

static const auto MAPPED_ALG_TYPES = [] {
	constexpr auto values = magic_enum::enum_values<AlgorithmType>();
	constexpr auto names = magic_enum::enum_names<AlgorithmType>();
	auto mapped = std::unordered_map<std::string, AlgorithmType>();
	for(auto i = 0u; i < values.size(); ++i) {
		mapped.insert_or_assign(names[i].data(), values[i]);
	}
	return mapped;
}();

auto main_logic(
	const common::MatSizes& sizes,
	const AlgorithmType& alg_type
) -> std::optional<main_rank::AlgStatistic> {

	if(sizes.first_cols != sizes.second_rows) {
		ERROR("Impossible to multiply");
	}

	const auto env = boost::mpi::environment();
	const auto world = boost::mpi::communicator();

	const auto subprocs_num = static_cast<unsigned>(world.size());
	if(subprocs_num <= 0) {
		ERROR("Number of tasks is less or equal zero");
	}

	const auto [tasks_num, step_length] = [&sizes, &subprocs_num] {
		const auto is_rows_less = sizes.first_rows < subprocs_num;
		const auto tasks_num = is_rows_less ? sizes.first_rows : subprocs_num;
		const auto step_length = is_rows_less ? 1 : tasks_num;
		return std::make_tuple(tasks_num, step_length);
	}();

	const auto rank = world.rank();
	const auto is_valid = rank <= tasks_num;
	const auto local = world.split(is_valid ? 0 : 1);

	if(not is_valid) {
		return std::nullopt;
	}

	if(rank == 0) {
		return main_rank::execute(local, alg_type, step_length, sizes);
	}
	child_rank::execute(local, alg_type, step_length, sizes);
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
			log_file << std::format("{{"
						   "\"procs_num\": {}, "
						   "\"mat_size\": {}, "
						   "\"alg_type\": \"{}\", "
						   "\"mpi_nanos\": {}, "
						   "\"single_nanos\": {}, "
						   "\"efficiency\": {} "
						   "}}", statistic.world_size, size,
			magic_enum::enum_name(alg_type), nano_mpi, nano_single, efficiency)
			<< std::endl;
		}
	});
}
