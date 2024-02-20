#include <lab_6/child_rank.hpp>
#include <lab_6/matrix.hpp>
#include <lab_6/common_tags.hpp>

#include <boost/mpi.hpp>

auto receive_matrices(
	const boost::mpi::communicator& world,
	const bool is_blocking
) -> std::tuple<Matrix, Matrix> {
	auto first = Matrix();
	auto second = Matrix();
	if(is_blocking) {
		world.recv(0, FROM_MAIN_THREAD_TAG, first);
		world.recv(0, FROM_MAIN_THREAD_TAG, second);
	} else {
		auto received = std::vector<boost::mpi::request> {
			world.irecv(0, FROM_MAIN_THREAD_TAG, first),
			world.irecv(0, FROM_MAIN_THREAD_TAG, second)
		};
		boost::mpi::wait_all(received.begin(), received.end());
	}
	return std::make_tuple(std::move(first), std::move(second));
}

auto child_rank(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned tasks_num,
	const bool is_blocking
) -> void {
	const auto rank = static_cast<unsigned>(world.rank());
	if(rank <= tasks_num) {
		return;
	}
	const auto [first, second] = receive_matrices(world, is_blocking);

	auto result = Matrix(size, size);
	auto curRow = rank - 1;
	const auto step = size < tasks_num ? 1 : tasks_num;
	while(curRow < size) {
		for(auto j = 0; j < size; ++j) {
			auto value = 0.0;
			for(auto k = 0; k < size; ++k) {
				value += first[curRow][k] * second[k][j];
			}
			result[curRow][j] = value;

		}
		curRow += step;
	}
	if(is_blocking) {
		world.send(0, FROM_TASK_THREAD_TAG, result);
	} else {
		world.isend(0, FROM_TASK_THREAD_TAG, result).wait();
	}
}