#include <mpi_mul/main_rank.hpp>
#include <mpi_mul/common.hpp>
#include <mpi_mul/matrix.hpp>
#include <mpi_mul/alg_type.hpp>
#include <mpi_mul/log_error.hpp>

#include <boost/mpi.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/shared_ptr.hpp>

namespace mpi_mul {

	auto send_tasks(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned tasks_num,
		const Matrix& first,
		const Matrix& second
	) -> void {
		const auto subprocs_num = world.size() - 1;
		const auto payload = boost::make_optional(
			Payload {
				.tasks_num = tasks_num,
				.matrices = std::make_pair(&first, &second)
			}
		);
		switch(alg_type) {
			case AlgorithmType::Blocking: {
				for(auto i = 0; i < subprocs_num; ++i) {
					// not all subprocesses may receive a task. Due to the fact that
					// number of rows in matrix is less than number of processes
					const auto message = i < tasks_num ? payload : boost::none;
					world.send(i + 1, FROM_MAIN_THREAD_TAG, message);
				}
				break;
			}
			case AlgorithmType::NonBlocking: {
				std::vector<boost::mpi::request> sent;
				sent.reserve(tasks_num * 2);
				for(auto i = 0; i < subprocs_num; ++i) {
					const auto message = i < tasks_num ? payload : boost::none;
					sent.push_back(world.isend(i + 1, FROM_MAIN_THREAD_TAG, message));
				}
				boost::mpi::wait_all(sent.begin(), sent.end());
				break;
			}
		}
	}

	auto receive_result(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned tasks_num,
		const unsigned first_rows
	) -> Matrix {

		auto result = std::vector<std::vector<double>>(first_rows);
		for(auto i = 0; i < tasks_num; ++i) {
			std::shared_ptr<std::vector<IndexedRow>> tempResult = nullptr;
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
			for(auto& [row_index, row] : *tempResult) {
				result[row_index] = std::move(row);
			}
		}
		return Matrix::unsafe_from_raw(std::move(result));
	}

	auto main_rank(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		const unsigned tasks_num,
		const Matrix& first,
		const Matrix& second
	) -> Matrix {
		send_tasks(world, alg_type, tasks_num, first, second);
		return receive_result(world, alg_type, tasks_num, first.get_rows());
	}
}