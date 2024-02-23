#include <mpi_mul/child_rank.hpp>
#include <mpi_mul/matrix.hpp>
#include <mpi_mul/common.hpp>
#include <mpi_mul/log_error.hpp>
#include <mpi_mul/alg_type.hpp>

#include <boost/mpi.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/shared_ptr.hpp>

namespace mpi_mul {

	auto receive_matrices(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type
	) -> boost::optional<Payload> {
		auto payload = boost::make_optional(Payload{});
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

	auto init_partial_matrix(
		const unsigned first_rows,
		const unsigned second_cols,
		const unsigned start_row,
		const unsigned step
	) -> std::vector<IndexedRow> {
		const auto total_rows = (first_rows - start_row) / step + 1;
		auto partial_mat = std::vector<IndexedRow>(total_rows);
		for(auto& r : partial_mat) {
			r.second.assign(second_cols, 0);
		}
		return partial_mat;
	}

	auto calculate_partial_result(
		const boost::mpi::communicator& world,
		const unsigned tasks_num,
		const Matrix& first,
		const Matrix& second
	) -> std::vector<IndexedRow> {

		const auto first_rows = first.get_rows();
		const auto second_cols = second.get_cols();
		const auto step = first_rows < tasks_num ? 1 : tasks_num;
		const auto start_row = static_cast<unsigned>(world.rank() - 1);
		auto result = init_partial_matrix(
			first_rows, second_cols, start_row, step
		);
		const auto second_rows = second.get_rows();
		for(auto row_index = start_row, i = 0u;
			row_index < first_rows; row_index += step, ++i) {

			auto& [index, row] = result[i];
			index = row_index;
			for(auto j = 0; j < second_cols; ++j) {
				auto value = 0.0;
				for(auto k = 0; k < second_rows; ++k) {
					value += first[row_index][k] * second[k][j];
				}
				row[j] = value;
			}
		}

		return result;
	}

	auto send_partial_result(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type,
		std::vector<IndexedRow>&& result) -> void {

		const auto result_ptr =
			std::make_shared<std::vector<IndexedRow>>(std::move(result));
		switch(alg_type) {
			case AlgorithmType::Blocking: {
				world.send(0, FROM_TASK_THREAD_TAG, result_ptr);
				break;
			}
			case AlgorithmType::NonBlocking: {
				world.isend(0, FROM_TASK_THREAD_TAG, result_ptr).wait();
				break;
			}
		}
	}

	auto child_rank(
		const boost::mpi::communicator& world,
		const AlgorithmType& alg_type
	) -> void {
		if(const auto payload = receive_matrices(world, alg_type)) {
			const auto& [tasks_num, matrices] = payload.value();
			auto partial_result = calculate_partial_result(
				world, tasks_num, *matrices.first, *matrices.second
			);
			send_partial_result(world, alg_type, std::move(partial_result));
		}
	}
}