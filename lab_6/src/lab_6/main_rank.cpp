#include <lab_6/main_rank.hpp>
#include <lab_6/matrix_factory.hpp>
#include <lab_6/common_tags.hpp>
#include <lab_6/log_error.hpp>
#include <lab_6/alg_type.hpp>

#include <boost/mpi.hpp>

#include <chrono>

static constexpr auto MIN_VAL = 0;
static constexpr auto MAX_VAL = 1;

using MatrixPair = std::array<Matrix, 2>;

auto send_tasks(
	const boost::mpi::communicator& world,
	const unsigned taks_num,
	const AlgorithmType& alg_type,
	const MatrixPair& matrix_pair
) -> void {
	switch(alg_type) {
		case AlgorithmType::Blocking: {
			for(auto i = 0; i < taks_num; ++i) {
				world.send(i + 1, FROM_MAIN_THREAD_TAG, matrix_pair);
			}
			break;
		}
		case AlgorithmType::NonBlocking: {
			std::vector<boost::mpi::request> sent;
			sent.reserve(taks_num * 2);
			for(auto i = 0; i < taks_num; ++i) {
				sent.push_back(world.isend(i + 1, FROM_MAIN_THREAD_TAG, matrix_pair));
			}
			boost::mpi::wait_all(sent.begin(), sent.end());
			break;
		}
	}
}

auto receive_result(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned taks_num,
	const AlgorithmType& alg_type
) -> Matrix {

	auto result = Matrix(size, size);
	for(auto i = 0; i < taks_num; ++i) {
		auto tempResult = Matrix(size, size);
		switch(alg_type) {
			case AlgorithmType::Blocking: {
				world.recv(i + 1, FROM_TASK_THREAD_TAG, tempResult);
				break;
			}
			case AlgorithmType::NonBlocking: {
				world.irecv(i + 1, FROM_TASK_THREAD_TAG, tempResult).wait();
				break;
			}
		}
		result += tempResult;
	}
	return result;
}

auto multiply_mpi(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned taks_num,
	const AlgorithmType& alg_type,
	const MatrixPair& matrix_pair
) -> std::tuple<std::chrono::system_clock::duration, Matrix> {

	const auto start_time = std::chrono::system_clock::now();
	send_tasks(world, taks_num, alg_type, matrix_pair);
	auto result = receive_result(world, size, taks_num, alg_type);
	const auto end_time = std::chrono::system_clock::now();
	const auto duration = end_time - start_time;
	return std::make_tuple(duration, std::move(result));
}

auto multiply_single(
	const MatrixPair& matrix_pair
) -> std::tuple<std::chrono::system_clock::duration, Matrix> {
	const auto start_time = std::chrono::system_clock::now();
	const auto& [first, second] = matrix_pair;
	auto result = first * second;
	const auto end_time = std::chrono::system_clock::now();
	const auto duration = end_time - start_time;
	return std::make_tuple(duration, std::move(result));
}

auto generate_matrices(
	const unsigned size
) -> MatrixPair {
	auto factory = MatrixFactory();
	return {
		factory.generate(size, size, MIN_VAL, MAX_VAL),
		factory.generate(size, size, MIN_VAL, MAX_VAL)
	};
}

auto main_rank(
	const boost::mpi::communicator& world,
	const unsigned size,
	const unsigned taks_num,
	const AlgorithmType& alg_type
) -> void {
	const auto matrix_pair = generate_matrices(size);
	const auto [duration_mpi, result_mpi] = multiply_mpi(
		world, size, taks_num, alg_type, matrix_pair);
	const auto [duration_single, result_single] = multiply_single(matrix_pair);
	if(result_mpi != result_single) {
		ERROR("MPI does not match single version");
	}
}
