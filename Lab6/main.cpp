#include <iostream>
#include <array>
#include <vector>
#include <mpi/mpi.h>

int main(int argc, char* argv[]) {
	using matrix = std::vector<std::vector<double>>;
	
	auto matrixSize = 500;

	auto rank = 0;
	auto size = 0;
	
	auto name = std::array<char, 80>();
	auto length = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	MPI_Get_processor_name(&name[0], &length);
	
	std::cout << "Hello, World!" << std::endl;
	
	MPI_Finalize();
	return 0;
}
