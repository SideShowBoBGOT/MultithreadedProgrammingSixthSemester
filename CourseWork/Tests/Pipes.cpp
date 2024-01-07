#include <thread>
#include <iostream>
#include <ParallelBFS/TPipes.hpp>

int main() {
	const auto [w, r] = bfs::pipes::TPipeChannel<int>::New();
	w.Write(4);
	auto value = r.Read();
	std::cout << value << std::endl;
}