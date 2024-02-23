#include <mpi_mul/mpi_mul.hpp>
#include <mpi_mul/log_error.hpp>
#include <mpi_mul/main_rank.hpp>
#include <mpi_mul/child_rank.hpp>

#include <boost/mpi.hpp>

namespace mpi_mul {

	auto multiply_main(
		const boost::mpi::communicator& world,
		const AlgorithmType alg_type,
		const Matrix& first,
		const Matrix& second
	) -> Matrix {
		const auto subprocs_num = world.size() - 1;
		if(subprocs_num <= 0) {
			ERROR("Number of tasks is less or equal zero");
		}
		const auto tasks_num = std::min(
			first.get_rows(), static_cast<unsigned>(subprocs_num)
		);
		return main_rank(world, alg_type, tasks_num, first, second);
	}

	auto multiply_child(
		const boost::mpi::communicator& world,
		const AlgorithmType alg_type
	) -> void {
		child_rank(world, alg_type);
	}

}