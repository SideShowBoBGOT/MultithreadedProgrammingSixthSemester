#ifndef MPI_MUL_HPP
#define MPI_MUL_HPP

#include <mpi_mul/matrix.hpp>
#include <mpi_mul/alg_type.hpp>

#include <boost/mpi.hpp>

#include <optional>

namespace mpi_mul {

	auto multiply_main(
		const boost::mpi::communicator& world,
		const AlgorithmType alg_type,
	    const Matrix& first,
	    const Matrix& second
	) -> Matrix;

	auto multiply_child(
		const boost::mpi::communicator& world,
		const AlgorithmType alg_type
	) -> void;

}

#endif //MPI_MUL_HPP
