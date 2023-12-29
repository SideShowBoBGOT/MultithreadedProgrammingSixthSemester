#include <gtest/gtest.h>
#include <ParallelBFS/TSequentialBFS.hpp>
#include <ParallelBFS/TPBFS.hpp>

std::unordered_map<unsigned, std::vector<unsigned>> Create2DGrid(const unsigned int size) {
	auto grid = std::unordered_map<unsigned , std::vector<unsigned>>();
	const auto totalSize = size * size;
	grid.reserve(totalSize);
	for(auto index = 0u; index < totalSize; ++index) {
		const auto x = static_cast<int>(index % size);
		const auto y = static_cast<int>(index / size);
		const auto utmost = static_cast<int>(size) - 1;
		auto neighbourIndexes = std::vector<unsigned>();
		for(auto deltaY = -1; deltaY <= 1; ++deltaY) {
			const auto newY = y + deltaY;
			if(newY < 0 || newY > utmost) continue;
			const auto base = static_cast<unsigned>(newY) * size;
			for(auto deltaX = -1; deltaX <= 1; ++deltaX) {
				if(deltaY == 0 && deltaX == 0) continue;
				const auto newX = x + deltaX;
				if(newX < 0 || newX > utmost) continue;
				const auto offset = static_cast<unsigned>(newX);
				neighbourIndexes.push_back(base + offset);
			}
		}
		grid.insert_or_assign(index, neighbourIndexes);
	}
	return grid;
}

template<typename... Args> requires (std::same_as<int, Args> && ...)
std::unordered_map<unsigned, bfs::AGraph<unsigned>> CreateGridMap(Args... args) {
	auto gridMap = std::unordered_map<unsigned, bfs::AGraph<unsigned>>();
	for(const auto size : {args...}) {
		const auto sizeUnsigned = static_cast<unsigned>(size);
		gridMap.insert_or_assign(sizeUnsigned, Create2DGrid(sizeUnsigned));
	}
	return gridMap;
}


class TTestBFSMixin {
	public:
	TTestBFSMixin()=default;

	protected:
	static std::unordered_map<unsigned, bfs::AGraph<unsigned>> s_vGridMap;
};

class TSequentialBFSTest : public ::testing::TestWithParam<unsigned>, public TTestBFSMixin {};
class TPBFSTest : public ::testing::TestWithParam<std::tuple<unsigned, unsigned>>, public TTestBFSMixin {};

#define INSTANTIATE_TEST_BFS(...)\
	std::unordered_map<unsigned, bfs::AGraph<unsigned>> TTestBFSMixin::s_vGridMap = CreateGridMap(__VA_ARGS__); \
	INSTANTIATE_TEST_SUITE_P(Benchmark, TSequentialBFSTest, testing::Values(__VA_ARGS__)); \
	INSTANTIATE_TEST_SUITE_P(Benchmark, TPBFSTest,\
		testing::Combine(\
			testing::Values(1, 2, 3, 4, 5, 6),\
			testing::Values(__VA_ARGS__)\
		)\
	);

INSTANTIATE_TEST_BFS(500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, 3000, 3250)
#undef INSTANTIATE_TEST_BFS

TEST_P(TSequentialBFSTest, Benchmark) {
	const auto size = GetParam();
	s_vGridMap

}




