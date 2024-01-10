#include <thread>
#include <iostream>
#include <ParallelBFS/TPipes.hpp>

int main() {
	auto [w, r] = bfs::TPipeChannel<int>();

	auto sender = std::jthread([ww=std::move(w)]() {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(4s);
		ww.Write(10);
	});

	auto listener = std::jthread([rr=std::move(r)]() {
		std::cout << rr.Read() << std::endl;
	});
}