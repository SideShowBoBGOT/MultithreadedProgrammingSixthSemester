#ifndef PARALLELBFS_TPARALLELBFSMIXIN_HPP
#define PARALLELBFS_TPARALLELBFSMIXIN_HPP

#include <atomic>
#include <thread>
#include <ParallelBFS/TBaseBFSMixin.hpp>

namespace bfs {

template<CBFSUsable T, typename Derived>
class TParallelBFSMixin : public TBaseBFSMixin<T, Derived> {
	protected:
	TParallelBFSMixin(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum);

	protected:
	using AVisitorMap = std::unordered_map<T, std::pair<std::atomic_flag, T>>;

	protected:
	AVisitorMap CreateVisitorMap() const;

	protected:
	const unsigned m_uThreadsNum = 0;
};

template<CBFSUsable T, typename Derived>
std::unordered_map<T, std::pair<std::atomic_flag, T>> TParallelBFSMixin<T, Derived>::CreateVisitorMap() const {
	auto visitorMap = std::unordered_map<T, std::pair<std::atomic_flag, T>>();
	visitorMap.reserve(this->m_refGraph.size());
	for(const auto& [key, _] : this->m_refGraph) {
		visitorMap.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple());
	}
	return visitorMap;
}

template<CBFSUsable T, typename Derived>
TParallelBFSMixin<T, Derived>::TParallelBFSMixin(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum)
	: m_uThreadsNum{std::min(threadsNum, std::jthread::hardware_concurrency())},
	TBaseBFSMixin<T, Derived>(graph, start, end) {}

}

#endif //PARALLELBFS_TPARALLELBFSMIXIN_HPP
