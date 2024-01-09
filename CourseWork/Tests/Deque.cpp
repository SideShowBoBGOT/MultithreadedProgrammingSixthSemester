#include <gtest/gtest.h>
#include <ParallelBFS/TDeque.hpp>

TEST(Deque, Loop) {
	auto deque = bfs::TDeque<int>();
	deque.Push({0, 1, 2, 3, 4});
	deque.Push({5, 6, 7, 8, 9});
	deque.Push({10, 11, 12, 13, 14});

	{
		auto begin = 0;
		deque.Loop(begin, 15, [&begin](const auto& el) {
			EXPECT_EQ(el, begin);
			++begin;
		});
		EXPECT_EQ(begin, 15);
	}

	{
		auto begin = 3;
		deque.Loop(begin, 12, [&begin](const auto& el) {
			EXPECT_EQ(el, begin);
			++begin;
		});
		EXPECT_EQ(begin, 12);
	}

}