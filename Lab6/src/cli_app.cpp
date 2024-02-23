#include <lab_6/cli_app.hpp>
#include <lab_6/matrix_factory.hpp>

#include <mpi_mul/mpi_mul.hpp>
#include <mpi_mul/log_error.hpp>

#include <magic_enum.hpp>

static constexpr auto MIN_VAL = 0;
static constexpr auto MAX_VAL = 1;

static const auto MAPPED_ALG_TYPES = []() {
	constexpr auto values = magic_enum::enum_values<mpi_mul::AlgorithmType>();
	constexpr auto names = magic_enum::enum_names<mpi_mul::AlgorithmType>();
	auto mapped = std::unordered_map<std::string, mpi_mul::AlgorithmType>();
	for(auto i = 0u; i < values.size(); ++i) {
		mapped.insert_or_assign(names[i].data(), values[i]);
	}
	return mapped;
}();

// auto multiply_single(
// 		const MatrixPtrPair& matrix_pair
// 	) -> std::tuple<std::chrono::system_clock::duration, Matrix> {
// 	const auto start_time = std::chrono::system_clock::now();
// 	const auto& [first, second] = matrix_pair;
// 	auto result = (*first) * (*second);
// 	const auto end_time = std::chrono::system_clock::now();
// 	const auto duration = end_time - start_time;
// 	return std::make_tuple(duration, std::move(result));
// }

auto generate_matrices(
	const unsigned size
) -> std::pair<mpi_mul::Matrix, mpi_mul::Matrix> {
	auto factory = MatrixFactory();
	return {
		factory.generate(size, size, MIN_VAL, MAX_VAL),
		factory.generate(size, size, MIN_VAL, MAX_VAL)
	};
}

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

		if(world.rank() == 0) {
			const auto [first, second] = generate_matrices(size);
			mpi_mul::multiply_main(world, algorithm_type, first, second);
		} else {
			mpi_mul::multiply_child(world, algorithm_type);
		}
		// const auto dur_single = static_cast<double>(duration_single.count());
		// const auto dur_mpi = static_cast<double>(duration_mpi.count());
		// const auto efficiency = dur_single / dur_mpi;
		// out << std::format("{}\t{}\t{}\t{}\t{}\t{}\n", world.size(), size,
		// 	magic_enum::enum_name(alg_type), duration_mpi, duration_single, efficiency);
	});
}