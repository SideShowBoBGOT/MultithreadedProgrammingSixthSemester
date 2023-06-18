#include <iostream>
#include <chrono>
#include <boost/mpi.hpp>

#include "MatrixFactory.h"
#include "Matrix.h"

namespace mpi = boost::mpi;

static constexpr auto FROM_MAIN_THREAD_TAG = 1;
static constexpr auto FROM_TASK_THREAD_TAG = 2;
static constexpr auto ROWS = 200;
static constexpr auto COLS = 200;
static constexpr auto MIN_VAL = 0;
static constexpr auto MAX_VAL = 1;

static const std::string NUMBER_OF_TASK_LE_ZERO = "Number of tasks is less or equal zero";
static const std::string MAIN_THREAD_TIME = "Main thread time: ";
static const std::string MILLIS = "ms";

static constexpr char LINE_FEED = '\n';

int main() {
	auto env = mpi::environment();
	auto world = mpi::communicator();
	
	auto tasksNum = world.size() - 1;
	if(tasksNum <= 0) {
		throw std::invalid_argument(NUMBER_OF_TASK_LE_ZERO);
	}

	auto isRowsLess = ROWS < tasksNum;
	auto workingTasksNum = isRowsLess ? ROWS : tasksNum;
	auto step = isRowsLess ? 1 : tasksNum;
	auto rank = world.rank();

	if(rank == 0) {		
		auto factory = MatrixFactory();
		auto first = factory.GenerateMatrix(ROWS, COLS, MIN_VAL, MAX_VAL);
		auto second = factory.GenerateMatrix(ROWS, COLS, MIN_VAL, MAX_VAL);
		auto result = Matrix(ROWS, COLS);
	
		auto start = std::chrono::high_resolution_clock::now();

		for(auto i = 0; i < workingTasksNum; ++i) {
			world.send(i + 1, FROM_MAIN_THREAD_TAG, first);
			world.send(i + 1, FROM_MAIN_THREAD_TAG, second);
		}

		for(auto i = 0; i < workingTasksNum; ++i) {
			auto tempResult = Matrix(ROWS, COLS);
			world.recv(i + 1, FROM_TASK_THREAD_TAG, tempResult);
			result.sum(tempResult);
		}

		// std::cout << first << LINE_FEED;
		// std::cout << second << LINE_FEED;
		// std::cout << result << LINE_FEED;
		
		auto end = std::chrono::high_resolution_clock::now();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << MAIN_THREAD_TIME;
		std::cout << millis << MILLIS << std::endl;
		
	} else if(rank <= workingTasksNum) {
		auto first = Matrix();
		auto second = Matrix();
		auto result = Matrix(ROWS, COLS);
		
		world.recv(0, FROM_MAIN_THREAD_TAG, first);
		world.recv(0, FROM_MAIN_THREAD_TAG, second);
		
		auto curRow = rank - 1;
		
        while(curRow < ROWS) {
            for(auto j = 0; j < COLS; ++j) {
                auto value = 0.0;
                for(auto k = 0; k < COLS; ++k) {
                    value += first[curRow][k] * second[k][j];
                }
                result[curRow][j] = value;

            }
            curRow += step;
        }
		
		world.send(0, FROM_TASK_THREAD_TAG, result);
	}

	return 0;
}