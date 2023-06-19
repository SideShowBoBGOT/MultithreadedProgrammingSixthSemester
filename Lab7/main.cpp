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

static constexpr auto MIN_VAL = 0;
static constexpr auto MAX_VAL = 1;

static const std::string NUMBER_OF_TASKS_LESS_EQUAL_ZERO = "Number of tasks is less or equal zero";
static const std::string INVALID_ALG_TYPE = "incorrect algorithm type";
static const std::string MAT_SIZE_NEGATIVE = "Matrix size can not be negative";

static const std::string MILLIS = "ms";

static const std::string LOG_FILE = "log.log";
static const std::string LOG_FORMAT = "%Message%";
static constexpr auto LOG_OPEN_MODE = std::ios_base::app;

static const std::string ONE_TO_MANY = "oneToMany";
static const std::string MANY_TO_MANY = "manyToMany";

static constexpr char TAB = '\t';

void initLog();
void oneToMany(const mpi::communicator& world, unsigned matSize);
void manyToMany(const mpi::communicator& world, unsigned matSize);

int main(int argc, char* argv[]) {

	auto env = mpi::environment();
	auto world = mpi::communicator();

	if( world.size() - 1 <= 0) {
		throw std::invalid_argument(NUMBER_OF_TASKS_LESS_EQUAL_ZERO);
	}

	initLog();

	auto matSize = static_cast<unsigned>(std::stoi(argv[1]));
	if(matSize < 0) {
		throw std::invalid_argument(MAT_SIZE_NEGATIVE);
	}
	auto algType = std::string(argv[2]);

	if(algType == ONE_TO_MANY) {
		oneToMany(world, matSize);
	} else if(algType == MANY_TO_MANY) {
		manyToMany(world, matSize);
	} else {
		throw std::invalid_argument(INVALID_ALG_TYPE);
	}

	return 0;
}

void oneToMany(const mpi::communicator& world, unsigned matSize) {
	Matrix first, second, result;

	if(rank == 0) {		
		auto factory = MatrixFactory();
		auto first = factory.GenerateMatrix(matSize, matSize, MIN_VAL, MAX_VAL);
		auto second = factory.GenerateMatrix(matSize, matSize, MIN_VAL, MAX_VAL);
	} else {
		first = Matrix(matSize)
	}

	auto start = std::chrono::high_resolution_clock::now();

	auto nodesNum = static_case<unsigned>(world.size());
	auto steps = matSize / nodesNum;
	auto extraSteps = matSize % nodesNum;

	mpi::broadcast(world, second, 0);
	mpi::scatter()

	auto end = std::chrono::high_resolution_clock::now();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	
	BOOST_LOG_TRIVIAL(info) << blockType << TAB << size
		<< TAB << world.size() << TAB << millis;
}

void manyToMany(const mpi::communicator& world, unsigned matSize) {

}

void initLog() {
	logging::add_file_log(
		LOG_FILE,
		logging::keywords::open_mode = LOG_OPEN_MODE,
		logging::keywords::format = LOG_FORMAT
	);
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info
    );
}
