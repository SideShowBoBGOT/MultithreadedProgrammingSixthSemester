#include <ParallelBFS/TCursor.hpp>
#include <ParallelBFS/TPipes.hpp>
#include <gtest/gtest.h>

#include <array>
#include <thread>

TEST(Cursor, Increment) {
	constexpr size_t maxValue = 100;
	bfs::TCursor cursor(maxValue);

	for(auto i = 0u; i < 1000; ++i) {
		EXPECT_EQ(cursor.GetIncrement(), i % maxValue);
	}
}

TEST(Cursor, MultithreadedIncrement) {
	const size_t threadsNum = std::jthread::hardware_concurrency();
	constexpr size_t maxValue = 100000;
	auto vals = std::array<std::atomic_size_t, maxValue>();

	auto cursor = bfs::TCursor(maxValue);

	{
		auto threads = std::vector<std::jthread>();
		for(auto i = 0u; i < threadsNum; ++i) {
			threads.emplace_back([&cursor, &maxValue, &vals] {
				for(size_t j = 0; j < maxValue; ++j) {
						vals[cursor.GetIncrement()].fetch_add(1);
					}
			});
		}
	}

	for(const auto& el : vals) {
		EXPECT_EQ(el, threadsNum);
	}
}