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

static const std::string ONE_TO_MANY = "one_to_many";
static const std::string MANY_TO_MANY = "many_to_many";

static constexpr char TAB = '\t';

void initLog();
void doMul(const mpi::communicator& world, unsigned matSize, const std::string& algType);

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

	if(algType != ONE_TO_MANY && algType != MANY_TO_MANY) {
		throw std::invalid_argument(INVALID_ALG_TYPE);
	}

	doMul(world, matSize, algType);

	return 0;
}

void doMul(const mpi::communicator& world, unsigned matSize, const std::string& algType) {
	auto rank = static_cast<unsigned>(world.rank());

	Matrix first, second;
	auto result = Matrix(matSize, matSize);

	if(rank == 0) {		
		auto factory = MatrixFactory();
		first = factory.GenerateMatrix(matSize, matSize, MIN_VAL, MAX_VAL);
		second = factory.GenerateMatrix(matSize, matSize, MIN_VAL, MAX_VAL);
	}		

	mpi::broadcast(world, second, 0);

	auto start = std::chrono::high_resolution_clock::now();

	auto nodesNum = static_cast<unsigned>(world.size());
	auto step = matSize / nodesNum;
	auto extraSteps = matSize % nodesNum;

	auto outProxy = Matrix();

	if(rank == 0) {
		auto firstInnerMat = first.innerMat();
		auto proxis = std::vector<Matrix>(nodesNum);
		for(auto i = 0u; i < proxis.size(); ++i) {
			auto from = firstInnerMat.begin() + i * step;
			auto to = from + step;
			std::copy(from, to, std::back_inserter(proxis[i].innerMat()));
		}
		mpi::scatter(world, proxis, outProxy, 0);
	} else {
		mpi::scatter(world, outProxy, 0);
	}

	auto fromRow = rank * step;
	for(unsigned i = fromRow, r = 0; (i < fromRow + step) && (r < step); ++i) {
		for(auto j = 0; j < matSize; ++j) {
			for(auto k = 0; k < matSize; ++k) {
				result[i][j] += outProxy[r][k] * second[k][j];
			}
		}
	}

	std::vector<Matrix> subResults;

	if(rank == 0) {
		// receive subResults to main thread from worker thread
		if(algType == ONE_TO_MANY) {
			mpi::gather(world, result, subResults, 0);
		} else if(algType == MANY_TO_MANY) {
			mpi::all_gather(world, result, subResults);
		}
		auto& sub = subResults.front();
		for(auto i = 1u; i < subResults.size(); ++i) {
			sub.sum(subResults[i]);
		}

		for(auto i = sub.rows() - extraSteps; i < sub.rows(); ++i) {
			for(auto j = 0; j < matSize; ++j) {
				for(auto k = 0; k < matSize; ++k) {
					sub[i][j] += first[i][k] * second[k][j];
				}
			}
		}

		// std::cout<<first<<std::endl;
		// std::cout<<second<<std::endl;
		// std::cout<<sub<<std::endl;

		auto end = std::chrono::high_resolution_clock::now();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	
		BOOST_LOG_TRIVIAL(info) << algType << TAB << matSize
			<< TAB << world.size() << TAB << millis;
	} else {
		// send subResult of worker thread to the main thread
		if(algType == ONE_TO_MANY) {
			mpi::gather(world, result, 0);
		} else if(algType == MANY_TO_MANY) {
			mpi::all_gather(world, result, subResults);
		}
	}
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
