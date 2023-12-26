#include <gtest/gtest.h>
#include <ParallelBFS/TSequentialBFS.hpp>
#include <ParallelBFS/TPBFS.hpp>

class T2DGridGenerator {

};


class TBFSTestFixture : public ::testing::Test {
	protected:
	static void SetUpTestSuite() {}
	static void TearDownTestSuite() {}

	protected:
};

TEST_F(TBFSTestFixture, SequentialBFS) {
	const auto graph = bfs::TGraph<int>{
		{0, {1, 2}},
		{1, {3}},
		{2, {3, 4}},
		{3, {5}},
		{4, {5, 6, 7}},
		{5, {}},
		{6, {5, 8}},
		{7, {6, 9}},
		{8, {}},
		{9, {8}}
	};

	const auto result = bfs::TSequentialBFS<int>::Do(graph, 0, 8);
	EXPECT_EQ(result, std::vector({0, 2, 4, 6, 8}));
}

TEST_F(TBFSTestFixture, PBFS) {
	const auto graph = bfs::TGraph<int>{
		{0, {1, 2}},
		{1, {3}},
		{2, {3, 4}},
		{3, {5}},
		{4, {5, 6, 7}},
		{5, {}},
		{6, {5, 8}},
		{7, {6, 9}},
		{8, {}},
		{9, {8}}
	};

	const auto result = bfs::TPBFS<int>::Do(graph, 0, 8, 5);
	EXPECT_EQ(result, std::vector({0, 2, 4, 6, 8}));
}