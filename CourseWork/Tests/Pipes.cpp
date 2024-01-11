#include <thread>
#include <gtest/gtest.h>
#include <ParallelBFS/TPipes.hpp>

TEST(Pipes, Transfer) {
	auto [w, r] = bfs::TPipeChannel<int>();

	auto sender = std::jthread([ww=std::move(w)]() {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(4s);
		ww.Write(10);
	});

	auto listener = std::jthread([rr=std::move(r)]() {
		EXPECT_EQ(rr.Read(), 10);
	});
}