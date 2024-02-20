#include <lab_6/main_rank.hpp>
#include <lab_6/matrix_factory.hpp>
#include <lab_6/common_tags.hpp>

#include <boost/mpi.hpp>

#include <chrono>

static constexpr auto MIN_VAL = 0;
static constexpr auto MAX_VAL = 1;

auto send_tasks(
	const boost::mpi::communicator& world,
	const unsigned taks_num,
	const bool is_blocking,
	const Matrix& first,
	const Matrix& second
) -> void {
	if(is_blocking) {
		for(auto i = 0; i < taks_num; ++i) {
			world.send(i + 1, FROM_MAIN_THREAD_TAG, first);
			world.send(i + 1, FROM_MAIN_THREAD_TAG, second);
		}
	} else {
		std::vector<boost::mpi::request> sent;
		sent.reserve(taks_num * 2);
		for(auto i = 0; i < taks_num; ++i) {
			sent.push_back(world.isend(i + 1, FROM_MAIN_THREAD_TAG, first));
			sent.push_back(world.isend(i + 1, FROM_MAIN_THREAD_TAG, second));
		}
		boost::mpi::wait_all(sent.begin(), sent.end());
	}
}

auto receive_result(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned taks_num,
	const bool is_blocking
) -> Matrix {
	auto result = Matrix(size, size);
	for(auto i = 0; i < taks_num; ++i) {
		auto tempResult = Matrix(size, size);
		if(is_blocking) {
			world.recv(i + 1, FROM_TASK_THREAD_TAG, tempResult);
		} else {
			world.irecv(i + 1, FROM_TASK_THREAD_TAG, tempResult).wait();
		}
		result.sum(tempResult);
	}
	return result;
}

auto multiply_mpi(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned taks_num,
	const bool is_blocking,
	const Matrix& first,
	const Matrix& second
) -> std::tuple<std::chrono::system_clock::duration, Matrix> {
	const auto start_time = std::chrono::system_clock::now();
	send_tasks(world, taks_num, is_blocking, first, second);
	auto result = receive_result(world, size, taks_num, is_blocking);
	const auto end_time = std::chrono::system_clock::now();
	const auto duration = end_time - start_time;
	return std::make_tuple(duration, std::move(result));
}

auto multiply_single(
	const Matrix& first,
	const Matrix& second
) -> std::tuple<std::chrono::system_clock::duration, Matrix> {
	const auto start_time = std::chrono::system_clock::now();
	auto result = first.mul(second);
	const auto end_time = std::chrono::system_clock::now();
	const auto duration = end_time - start_time;
	return std::make_tuple(duration, std::move(result));
}

auto generate_matrices(
	const unsigned size
) -> std::tuple<Matrix, Matrix> {
	auto factory = MatrixFactory();
	auto first = factory.generate(size, size, MIN_VAL, MAX_VAL);
	auto second = factory.generate(size, size, MIN_VAL, MAX_VAL);
	return std::make_tuple(std::move(first), std::move(second));
}

auto main_rank(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned taks_num,
	const bool is_blocking
) -> void {
	const auto [first, second] = generate_matrices(size);
	const auto [duration_mpi, result_mpi] = multiply_mpi(
		world, size, taks_num, is_blocking, first, second);
	const auto [duration_single, result_single] = multiply_single(first, second);

}
