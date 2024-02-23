#ifndef CLI_HPP
#define CLI_HPP

#include <mpi_mul/alg_type.hpp>

#include <CLI/CLI.hpp>

class CliApp : public CLI::App {
	public:
	CliApp();

	protected:
	mpi_mul::AlgorithmType algorithm_type;
	unsigned size;
};

#endif //CLI_HPP
