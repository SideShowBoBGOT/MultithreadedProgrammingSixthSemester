#define BOOST_LOG_DYN_LINK 1

#include <iostream>
#include <chrono>

#include <boost/mpi.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include "MatrixFactory.h"
#include "Matrix.h"

namespace mpi = boost::mpi;
namespace logging = boost::log;

static constexpr auto FROM_MAIN_THREAD_TAG = 1;
static constexpr auto FROM_TASK_THREAD_TAG = 2;
static constexpr auto MIN_VAL = 0;
static constexpr auto MAX_VAL = 1;

static const std::string NUMBER_OF_TASK_LE_ZERO = "Number of tasks is less or equal zero";
static const std::string MAIN_THREAD_TIME = "Main thread time: ";
static const std::string MILLIS = "ms";

static const std::string LOG_FILE = "log.log";
static const std::string LOG_FORMAT = "%Message%";
static constexpr auto LOG_OPEN_MODE = std::ios_base::app;

static const std::string BLOCKING = "blocking";

void init();

int main(int argc, char* argv[]) {
	init();

	auto size = static_cast<unsigned>(std::stoi(argv[1]));
	auto isBlocking = std::string(argv[2]) == BLOCKING;

	auto env = mpi::environment();
	auto world = mpi::communicator();
	
	auto tasksNum = world.size() - 1;
	if(tasksNum <= 0) {
		throw std::invalid_argument(NUMBER_OF_TASK_LE_ZERO);
	}

	auto isRowsLess = size < tasksNum;
	auto workingTasksNum = isRowsLess ? size : tasksNum;
	auto step = isRowsLess ? 1 : tasksNum;
	auto rank = world.rank();

	if(rank == 0) {		
		auto factory = MatrixFactory();
		auto first = factory.GenerateMatrix(size, size, MIN_VAL, MAX_VAL);
		auto second = factory.GenerateMatrix(size, size, MIN_VAL, MAX_VAL);
		auto result = Matrix(size, size);
	
		auto start = std::chrono::high_resolution_clock::now();

		for(auto i = 0; i < workingTasksNum; ++i) {
			if(isBlocking) {
				world.send(i + 1, FROM_MAIN_THREAD_TAG, first);
				world.send(i + 1, FROM_MAIN_THREAD_TAG, second);
			} else {
				world.isend(i + 1, FROM_MAIN_THREAD_TAG, first);
				world.isend(i + 1, FROM_MAIN_THREAD_TAG, second);
			}
			
		}

		for(auto i = 0; i < workingTasksNum; ++i) {
			auto tempResult = Matrix(size, size);
			if(isBlocking) {
				world.recv(i + 1, FROM_TASK_THREAD_TAG, tempResult);
			} else {
				world.irecv(i + 1, FROM_TASK_THREAD_TAG, tempResult);
			}
			
			result.sum(tempResult);
		}

		BOOST_LOG_TRIVIAL(info) << first;
		// std::cout << second << LINE_FEED;
		// std::cout << result << LINE_FEED;
		
		auto end = std::chrono::high_resolution_clock::now();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << MAIN_THREAD_TIME;
		std::cout << millis << MILLIS << std::endl;
		
	} else if(rank <= workingTasksNum) {
		auto first = Matrix();
		auto second = Matrix();
		auto result = Matrix(size, size);
		
		if(isBlocking) {
			world.recv(0, FROM_MAIN_THREAD_TAG, first);
			world.recv(0, FROM_MAIN_THREAD_TAG, second);
		} else {
			world.irecv(0, FROM_MAIN_THREAD_TAG, first);
			world.irecv(0, FROM_MAIN_THREAD_TAG, second);
		}
		
		auto curRow = rank - 1;
		
        while(curRow < size) {
            for(auto j = 0; j < size; ++j) {
                auto value = 0.0;
                for(auto k = 0; k < size; ++k) {
                    value += first[curRow][k] * second[k][j];
                }
                result[curRow][j] = value;

            }
            curRow += step;
        }
		
		if(isBlocking) {
			world.send(0, FROM_TASK_THREAD_TAG, result);
		} else {
			world.isend(0, FROM_TASK_THREAD_TAG, result);
		}
	}

	return 0;
}

void init() {
	logging::add_file_log(
		LOG_FILE,
		logging::keywords::open_mode = LOG_OPEN_MODE,
		logging::keywords::format = LOG_FORMAT
	);
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );
}