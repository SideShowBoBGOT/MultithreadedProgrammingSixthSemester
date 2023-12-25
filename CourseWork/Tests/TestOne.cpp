#include <gtest/gtest.h>
#include <ParallelBFS/TSequentialBFS.hpp>
#include <ParallelBFS/TPBFS.hpp>

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

	const auto bfs = bfs::TSequentialBFS<int>(graph, 0, 8);
	EXPECT_EQ(bfs.Execute(), std::vector({0, 2, 4, 6, 8}));
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

	const auto bfs = bfs::TPBFS<int>(graph, 0, 8, 5);
	EXPECT_EQ(bfs.Execute(), std::vector({0, 2, 4, 6, 8}));
}

//template<typename T>
//struct Guard {
//	Guard(T* o) : obj {o} {
//		std::cout << "Created" << std::endl;
//	}
//	~Guard() {
//		std::cout << "Destructed" << std::endl;
//	}
//	T* operator->() {
//		return obj;
//	}
//
//	protected:
//	T* obj;
//};
//
//template<typename T>
//struct Base {
//	T obj;
//	Guard<T> Create() {
//		return Guard<T>(&obj);
//	}
//};
//
//struct Some {
//	void Do() {
//		std::cout << "do" << std::endl;
//	}
//};
//
//TEST_F(TBFSTestFixture, oooo) {
//	auto b = Base<Some>();
//	b.Create()->Do();
//}