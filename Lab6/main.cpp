#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <boost/mpi.hpp>

#include "LabStuff/MatrixFactory.h"
#include "LabStuff/Matrix.h"

namespace mpi = boost::mpi;

int main() {
	auto env = mpi::environment();
	auto world = mpi::communicator();

	auto rows = 5;
	auto cols = 5;
	auto minVal = 0;
	auto maxVal = 1;
	
	auto isRowsLess = rows < world.size();
	auto totalThreads = isRowsLess ? rows : world.size();
	auto step = isRowsLess ? 1 : world.size();

	if(world.rank() == 0) {
		
		auto factory = MatrixFactory();
		auto first = factory.GenerateMatrix(rows, cols, minVal, maxVal);
		auto second = factory.GenerateMatrix(rows, cols, minVal, maxVal);
		auto result = Matrix(rows, cols);
	
		auto start = std::chrono::high_resolution_clock::now();
		for(auto i = 0; i < totalThreads; ++i) {
			world.send(i + 1, 1, first);
			world.send(i + 1, 1, second);
		}
		
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "Main thread time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
		
	} else {
		auto first = Matrix();
		auto second = Matrix();
		
		world.recv(0, 1, first);
		world.recv(0, 1, second);
		
		auto curRow = world.rank();
		
        while(curRow < rows) {
            for(auto j = 0; j < cols; ++j) {
                auto value = 0.0;
                for(auto k = 0; k < cols; ++k) {
                    value += first[curRow][k] * second[k][j];
                }
                result.setAt(value, curRow, j);
            }
            curRow += step;
        }
	}
	
	return 0;
}