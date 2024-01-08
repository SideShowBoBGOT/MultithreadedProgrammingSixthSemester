#include <gtest/gtest.h>
#include <ParallelBFS/TDeque.hpp>

TEST(Deque, Loop) {
	auto deque = bfs::TDeque<int>();
	EXPECT_EQ(deque.begin(), deque.begin());
	EXPECT_EQ(deque.end(), deque.end());

	auto vOne = std::vector{0, 1, 2, 3, 4};
	auto vTwo = std::vector{5, 6, 7, 8, 9};
	deque.push_back(vOne);
	deque.push_back(vTwo);

	EXPECT_EQ(deque.end(), deque.end());

	auto totalIterations = 0;
	for(auto el : deque) {
		EXPECT_EQ(el, totalIterations);
		++totalIterations;
	}
	EXPECT_EQ(totalIterations, 10);
}

TEST(Deque, ErrorIfEmpty) {
	auto deque = bfs::TDeque<int>();
	EXPECT_ANY_THROW(deque.begin()++);
}

TEST(Deque, AddValue) {
	auto deque = bfs::TDeque<int>();
	deque.push_back({0, 1, 2, 3, 4});
	deque.push_back({5, 6, 7, 8, 9});
	deque.push_back({10, 11, 12, 13, 14});
	EXPECT_EQ(deque.size(), 15);
	auto begin = deque.begin();
	EXPECT_EQ(*begin, 0);
	for(auto i = std::size_t(0); i < deque.size(); ++i) {
		const auto it = begin + i;
		EXPECT_EQ(*it, i);
	}
}