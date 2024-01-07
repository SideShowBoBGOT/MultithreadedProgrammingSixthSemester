#include <format>
#include <fstream>
#include <filesystem>
#include <gtest/gtest.h>
#include <ParallelBFS/TSequentialBFS.hpp>
#include <ParallelBFS/TSharedBFS.hpp>

class TTestBFSFixture : public ::testing::Test {
	protected:
	static std::unordered_map<unsigned, std::vector<unsigned>> Create2DGrid(const unsigned int size);
	static bool IsPathValid(const std::vector<unsigned int>& path, const bfs::AGraph<unsigned int>& graph);
	static unsigned GetLastIndex(const unsigned size);
	static void WriteToReport(const std::string& str);
};

std::unordered_map<unsigned, std::vector<unsigned>> TTestBFSFixture::Create2DGrid(const unsigned int size) {
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

bool TTestBFSFixture::IsPathValid(const std::vector<unsigned int>& path, const bfs::AGraph<unsigned int>& graph) {
	for(const auto& [start, end] : path | std::views::pairwise) {
		const auto it = graph.find(start);
		if(it == graph.end()) {
			return false;
		}
		const auto isContain = std::ranges::contains(it->second, end);
		if(not isContain) {
			return false;
		}
	}
	return true;
}

unsigned TTestBFSFixture::GetLastIndex(const unsigned size) {
	return (size - 1) * size + size - 1;
}

void TTestBFSFixture::WriteToReport(const std::string& str) {
	std::ofstream("Benchmark.txt", std::ios::app) << str << std::endl;
}

TEST_F(TTestBFSFixture, Test) {
	constexpr auto totalRepeats = 5u;
	const auto sizes = std::vector<unsigned>{2750, 2875, 3000, 3125, 3250, 3500, 3635, 3750, 3875, 4000};
	const auto threadsNums = std::vector<unsigned>{3, 4, 5};
	for(const auto size : sizes) {
		const auto grid = Create2DGrid(size);
		const auto lastIndex = GetLastIndex(size);
		for(auto i = 0u; i < totalRepeats; ++i) {
			const auto sequentialMillis = static_cast<double>([&grid, &lastIndex, &size]() {
				const auto start = std::chrono::system_clock::now();
				const auto result = bfs::TSequentialBFS<unsigned>::Do(grid, 0, lastIndex);
				const auto delay = std::chrono::system_clock::now() - start;
				const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(delay).count();
				EXPECT_TRUE(IsPathValid(result.value(), grid));
				WriteToReport(std::format("{{ name: {}, size: {}, milliseconds: {} }}", "Sequential", size, millis));
				return millis;
			}());
			for(const auto threadsNum : threadsNums) {
				const auto start = std::chrono::system_clock::now();
				const auto result = bfs::TSharedBFS<unsigned>::Do(grid, 0, lastIndex, threadsNum);
				const auto delay = std::chrono::system_clock::now() - start;
				const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(delay).count();
				EXPECT_TRUE(IsPathValid(result.value(), grid));
				WriteToReport(std::format("{{ name: {}, size: {}, threadsNum: {}, milliseconds: {}, acceleration: {} }}",
					"Shared", size, threadsNum, millis, sequentialMillis / static_cast<double>(millis)));
			}
		}
	}
}