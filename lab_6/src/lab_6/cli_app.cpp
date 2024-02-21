#include <lab_6/cli_app.hpp>
#include <lab_6/main_rank.hpp>
#include <lab_6/child_rank.hpp>
#include <lab_6/log_error.hpp>

#include <boost/mpi.hpp>
#include <magic_enum.hpp>

static const auto MAPPED_ALG_TYPES = []() {
	constexpr auto values = magic_enum::enum_values<AlgorithmType>();
	constexpr auto names = magic_enum::enum_names<AlgorithmType>();
	auto mapped = std::unordered_map<std::string, AlgorithmType>();
	for(auto i = 0u; i < values.size(); ++i) {
		mapped.insert_or_assign(names[i].data(), values[i]);
	}
	return mapped;
}();

CliApp::CliApp() : CLI::App("lab_6") {
	add_option("--size,-s", size, "Matrix size")
		->required(true)
		->check(CLI::PositiveNumber);
	add_option("--type,-t", algorithm_type, "Algorithm type")
		->transform(CLI::CheckedTransformer(MAPPED_ALG_TYPES, CLI::ignore_case))
		->required(true);

	parse_complete_callback([this] {
		const auto env = boost::mpi::environment();
		const auto world = boost::mpi::communicator();
		const auto child_procs = world.size() - 1;
		if(child_procs <= 0) {
			ERROR("Number of tasks is less or equal zero");
		}
		const auto tasks_num = std::min(size, static_cast<unsigned>(child_procs));
		if(world.rank() == 0) {
			main_rank(world, size, tasks_num, algorithm_type);
		} else {
			child_rank(world, size, tasks_num, algorithm_type);
		}
	});
}