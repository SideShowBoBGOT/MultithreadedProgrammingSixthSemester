#include <lab_6/cli_app.hpp>

int main(int argc, char *argv[]) {
	auto cli = CliApp();
	CLI11_PARSE(cli, argc, argv);
	return 0;
}