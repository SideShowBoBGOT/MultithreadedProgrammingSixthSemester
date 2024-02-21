#include <lab_6/main_rank.hpp>
#include <lab_6/child_rank.hpp>
#include <lab_6/log_error.hpp>

#include <boost/mpi.hpp>

#include <chrono>

static const std::string BLOCKING = "blocking";

int main(int argc, char* argv[]) {
	const auto size = static_cast<unsigned>(std::stoi(argv[1]));
	const auto blockType = std::string(argv[2]);
	const auto is_blocking = blockType == BLOCKING;
	const auto env = boost::mpi::environment(argc, argv);
	const auto world = boost::mpi::communicator();
	const auto child_procs = world.size() - 1;
	if(child_procs <= 0) {
		ERROR("Number of tasks is less or equal zero");
	}
	const auto tasks_num = std::min(size, static_cast<unsigned>(child_procs));
	if(world.rank() == 0) {
		main_rank(world, size, tasks_num, is_blocking);
	} else {
		child_rank(world, size, tasks_num, is_blocking);
	}
	return 0;
}