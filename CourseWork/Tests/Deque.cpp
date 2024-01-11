#include <gtest/gtest.h>
#include <ParallelBFS/TDeque.hpp>

class TDequeTest
	: public testing::TestWithParam<std::array<int, 2>> {
	protected:
	static void SetUpTestSuite();
	static bfs::TDeque<int> s_vDeque;
};

void TDequeTest::SetUpTestSuite() {
	s_vDeque.Push({0, 1, 2, 3, 4});
	s_vDeque.Push({5, 6, 7, 8, 9});
	s_vDeque.Push({10, 11, 12, 13, 14});
}

bfs::TDeque<int> TDequeTest::s_vDeque = bfs::TDeque<int>();

INSTANTIATE_TEST_SUITE_P(Loop, TDequeTest,
	testing::Values(std::array{0, 15}, std::array{3, 12}));

TEST_P(TDequeTest, Loop) {
	auto [beginIt, endIt] = GetParam();
	s_vDeque.Loop(beginIt, endIt, [&beginIt](const auto& el) {
		EXPECT_EQ(el, beginIt);
		++beginIt;
	});
	EXPECT_EQ(beginIt, endIt);
}