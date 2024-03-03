#ifndef CLI_HPP
#define CLI_HPP

#include <lab_7/alg_type.hpp>

#include <CLI/CLI.hpp>

class CliApp : public CLI::App {
	public:
	CliApp();

	public:
	AlgorithmType alg_type;
	unsigned size;
};

#endif //CLI_HPP
