#include <format>
#include <fstream>
#include <filesystem>
#include <gtest/gtest.h>
#include <ParallelBFS/TSequentialBFS.hpp>
#include <ParallelBFS/TPBFS.hpp>

std::unordered_map<unsigned, std::vector<unsigned>> Create2DGrid(const unsigned int size) {
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

template<typename... Args> requires (std::same_as<int, Args> && ...)
std::unordered_map<unsigned, bfs::AGraph<unsigned>> CreateGridMap(Args... args) {
	auto gridMap = std::unordered_map<unsigned, bfs::AGraph<unsigned>>();
	for(const auto size : {args...}) {
		const auto sizeUnsigned = static_cast<unsigned>(size);
		gridMap.insert_or_assign(sizeUnsigned, Create2DGrid(sizeUnsigned));
	}
	return gridMap;
}

// GridMap takes much time to be created. If its resources are
// contained inside global static variable, I will get a timeout error
// from googletest. So the solution is to delay the construction of this
// resource and only have one instance of it. Therefore, Singleton is
// the best way to do this.

// Also this class is used for other things to be done only once, such as:
// creation of benchmark file
class TTestBenchmarkSetUpper {
	public:
	static const TTestBenchmarkSetUpper* GetInstance();
	const std::unordered_map<unsigned, bfs::AGraph<unsigned>>& GridMap() const;
	const std::filesystem::path& OutputFilePath() const;

	protected:
	TTestBenchmarkSetUpper();

	protected:
	template<typename... Sizes> requires (std::same_as<int, Sizes> && ...)
	void Init(Sizes... args);

	protected:
	std::unordered_map<unsigned, bfs::AGraph<unsigned>> m_vGridMap;
	std::filesystem::path m_sOutputFilePath;

	protected:
	static std::unique_ptr<TTestBenchmarkSetUpper> s_pInstance;
};

std::unique_ptr<TTestBenchmarkSetUpper> TTestBenchmarkSetUpper::s_pInstance = nullptr;

const TTestBenchmarkSetUpper* TTestBenchmarkSetUpper::GetInstance() {
	// Enable make_unique use protected constructor of TTestBenchmarkSetUpper;
	struct TEnableMaker : public TTestBenchmarkSetUpper { using TTestBenchmarkSetUpper::TTestBenchmarkSetUpper; };
	if(not s_pInstance) {
		s_pInstance = std::make_unique<TEnableMaker>();
	}
	return s_pInstance.get();
}

const std::unordered_map<unsigned, bfs::AGraph<unsigned>>& TTestBenchmarkSetUpper::GridMap() const {
	return m_vGridMap;
}

const std::filesystem::path& TTestBenchmarkSetUpper::OutputFilePath() const {
	return m_sOutputFilePath;
}

template<typename... Sizes> requires (std::same_as<int, Sizes> && ...)
void TTestBenchmarkSetUpper::Init(Sizes... args) {
	m_vGridMap = CreateGridMap(args...);
	m_sOutputFilePath = "Benchmark.txt";
	std::filesystem::remove(m_sOutputFilePath);
}

class TTestBFSGeneral : public testing::Test {
	protected:
	virtual void TearDown() override;

	protected:
	static void SetUpTestSuite();
	static void TearDownTestSuite();

	protected:
	virtual std::string GenerateReport(const long millis) const=0;

	protected:
	static unsigned GetLastIndex(const unsigned size);

	protected:
	std::chrono::time_point<std::chrono::system_clock> m_xStart;
	std::chrono::time_point<std::chrono::system_clock> m_xEnd;
};

void TTestBFSGeneral::TearDown() {
	Test::TearDown();
	const auto delay = std::chrono::system_clock::now() - m_xStart;
	const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(delay).count();
	auto file = std::ofstream(TTestBenchmarkSetUpper::GetInstance()->OutputFilePath(), std::ios::app);
	file << GenerateReport(millis) << std::endl;
}

void TTestBFSGeneral::SetUpTestSuite() {
	// Create instance of TTestBenchmarkSetUpper
	TTestBenchmarkSetUpper::GetInstance();
}

void TTestBFSGeneral::TearDownTestSuite() {}

unsigned TTestBFSGeneral::GetLastIndex(const unsigned int size) {
	return (size - 1) * size + size - 1;
}

template <typename T>
class TestBFSMixin : public TTestBFSGeneral, public testing::WithParamInterface<T> {};

class TSequentialBFSTest : public TestBFSMixin<unsigned> {
	protected:
	virtual std::string GenerateReport(const long millis) const override;
};

std::string TSequentialBFSTest::GenerateReport(const long millis) const {
	return std::format("{{ name: {}, size: {}, milliseconds: {} }}",
		"TSequentialBFSTest", GetParam(), millis);
}

class TPBFSTest : public TestBFSMixin<std::tuple<unsigned, unsigned>> {
	protected:
	virtual std::string GenerateReport(const long millis) const override;
};

std::string TPBFSTest::GenerateReport(const long millis) const {
	return std::format("{{ name: {}, size: {}, threadsNum: {}, milliseconds: {} }}", "TPBFSTest",
		std::get<1>(GetParam()), std::get<0>(GetParam()), millis);
}

TEST_P(TSequentialBFSTest, Benchmark) {
	m_xStart = std::chrono::system_clock::now();
	const auto size = GetParam();
	const auto lastIndex = GetLastIndex(size);
	const auto& grid = TTestBenchmarkSetUpper::GetInstance()->GridMap().at(size);
	const auto result = bfs::TSequentialBFS<unsigned>::Do(grid, 0, lastIndex);
	m_xEnd = std::chrono::system_clock::now();
	EXPECT_TRUE(result.has_value());
}

TEST_P(TPBFSTest, Benchmark) {
	m_xStart = std::chrono::system_clock::now();
	const auto threadsNum = std::get<0>(GetParam());
	const auto size = std::get<1>(GetParam());
	const auto lastIndex = GetLastIndex(size);
	const auto& grid = TTestBenchmarkSetUpper::GetInstance()->GridMap().at(size);
	const auto result = bfs::TPBFS<unsigned>::Do(grid, 0, lastIndex, threadsNum);
	m_xEnd = std::chrono::system_clock::now();
	EXPECT_TRUE(result.has_value());

}

#define INSTANTIATE_TEST_BFS(...) \
    TTestBenchmarkSetUpper::TTestBenchmarkSetUpper() {\
		Init(__VA_ARGS__);\
	}\
	INSTANTIATE_TEST_SUITE_P(Benchmark, TSequentialBFSTest, testing::Values(__VA_ARGS__)); \
	INSTANTIATE_TEST_SUITE_P(Benchmark, TPBFSTest,\
		testing::Combine(\
			testing::Values(1, 2, 3, 4, 5),\
			testing::Values(__VA_ARGS__)\
		)\
	);

INSTANTIATE_TEST_BFS(2750, 3000)


//INSTANTIATE_TEST_BFS(10, 20, 30, 40, 50)
#undef INSTANTIATE_TEST_BFS






