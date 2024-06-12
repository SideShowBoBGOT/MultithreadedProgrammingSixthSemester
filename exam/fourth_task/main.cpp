#include <boost/mpi.hpp>
#include <CLI/CLI.hpp>

static constexpr int MAIN_RANK = 0;
constexpr auto FROM_TASK_THREAD_TAG = 2;

size_t comm_size(const boost::mpi::communicator& world) {
	return static_cast<size_t>(world.size());
}

int calc_local_sum(const boost::mpi::communicator& world, const std::vector<int>& vec) {
	const auto chunk_size = vec.size() / comm_size(world);

	const auto start_shift = world.rank() * chunk_size;
	const auto end_shift = (world.rank() == (world.size() - 1)) ? vec.size() : std::min(start_shift + chunk_size, vec.size());


	const auto start_it = std::next(std::cbegin(vec), static_cast<long>(start_shift));
	const auto end_it = std::next(std::cbegin(vec), static_cast<long>(end_shift));

	return std::accumulate(start_it, end_it, 0, std::plus());
}

void calc_global_sum(const boost::mpi::communicator& world, const std::vector<int>& vec) {
	if(world.rank() == MAIN_RANK) {
		auto recv_requests = std::vector<boost::mpi::request>();
		auto local_sums = std::vector(world.size() - 1, 0);
		for(auto i = 1; i < world.size(); ++i) {
			recv_requests.push_back(world.irecv(i, FROM_TASK_THREAD_TAG, local_sums[i-1]));
		}
		boost::mpi::wait_all(std::begin(recv_requests), std::end(recv_requests));
		const auto global_sum = std::accumulate(
			std::begin(local_sums), std::end(local_sums), calc_local_sum(world, vec), std::plus<>());
		std::cout << global_sum;
	} else {
		world.send(MAIN_RANK, FROM_TASK_THREAD_TAG, calc_local_sum(world, vec));
	}
}

void main_logic(const std::vector<int>& vec) {

	const auto env = boost::mpi::environment();
	const auto world = boost::mpi::communicator();

	const auto needed_procs_num = std::min(static_cast<size_t>(world.size()), vec.size());
	const auto is_needed = world.rank() < needed_procs_num;
	const auto local_world = world.split(is_needed);

	if(not is_needed) {
		return;
	}

	calc_global_sum(world, vec);
}

class CliApp final : public CLI::App {
	public:
	CliApp();

	public:
	std::vector<int> vec;
};

CliApp::CliApp() : CLI::App("exam_mpi") {
	add_option("--vector,-v", vec, "Vector of elments")
		->required(true);

	parse_complete_callback([this] {
		main_logic(vec);
	});
}


int main(int argc, char *argv[]) {
	auto cli = CliApp();
	CLI11_PARSE(cli, argc, argv);
	return 0;
}