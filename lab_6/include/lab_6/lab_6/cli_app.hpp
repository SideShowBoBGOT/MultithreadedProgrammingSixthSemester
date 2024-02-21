#ifndef CLI_HPP
#define CLI_HPP

#include <CLI/CLI.hpp>
#include <lab_6/alg_type.hpp>

class CliApp : public CLI::App {
	public:
	CliApp();

	protected:
	AlgorithmType algorithm_type;
	unsigned size;
};

#endif //CLI_HPP
