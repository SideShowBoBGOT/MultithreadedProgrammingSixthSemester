#include <gtest/gtest.h>
#include <ParallelBFS/TSequentialBFS.hpp>
#include <ParallelBFS/TPBFS.hpp>

#include <ranges>

static std::unordered_map<unsigned, std::vector<unsigned>> Create2DGrid(const unsigned size) {
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

class TBFSTestFixture : public ::testing::Test {
	protected:
	static void SetUpTestSuite() {
		//s_umGrid = Create2DGrid(s_uGridSize);
	}
	static void TearDownTestSuite() {}

	protected:
	static constexpr auto s_uGridSize = 1000u;
	static std::unordered_map<unsigned, std::vector<unsigned>> s_umGrid;
};

std::unordered_map<unsigned, std::vector<unsigned>> TBFSTestFixture::s_umGrid = Create2DGrid(s_uGridSize);

//TEST_F(TBFSTestFixture, SequentialBFS_Simple) {
//	const auto graph = bfs::TGraph<int>{
//		{0, {1, 2}},
//		{1, {3}},
//		{2, {3, 4}},
//		{3, {5}},
//		{4, {5, 6, 7}},
//		{5, {}},
//		{6, {5, 8}},
//		{7, {6, 9}},
//		{8, {}},
//		{9, {8}}
//	};
//
//	const auto result = bfs::TSequentialBFS<int>::Do(graph, 0, 8);
//	EXPECT_EQ(result, std::vector({0, 2, 4, 6, 8}));
//}
//
//TEST_F(TBFSTestFixture, PBFS_Simple) {
//	const auto graph = bfs::TGraph<int>{
//		{0, {1, 2}},
//		{1, {3}},
//		{2, {3, 4}},
//		{3, {5}},
//		{4, {5, 6, 7}},
//		{5, {}},
//		{6, {5, 8}},
//		{7, {6, 9}},
//		{8, {}},
//		{9, {8}}
//	};
//
//	const auto result = bfs::TPBFS<int>::Do(graph, 0, 8, 5);
//	EXPECT_EQ(result, std::vector({0, 2, 4, 6, 8}));
//}

TEST_F(TBFSTestFixture, SimpleSequentialBFS_Grid2D) {
	constexpr auto lastIndex = (s_uGridSize - 1) * s_uGridSize + s_uGridSize - 1;
	const auto result = bfs::TSequentialBFS<unsigned>::Do(s_umGrid, 0, lastIndex);
	//const auto result = bfs::TPBFS<unsigned>::Do(s_umGrid, 0, lastIndex, 1);
	EXPECT_EQ(true, false);
}

TEST_F(TBFSTestFixture, PBFS_Grid2D) {
	constexpr auto lastIndex = (s_uGridSize - 1) * s_uGridSize + s_uGridSize - 1;
	const auto result = bfs::TPBFS<unsigned>::Do(s_umGrid, 0, lastIndex, 4);
	EXPECT_EQ(true, false);
}