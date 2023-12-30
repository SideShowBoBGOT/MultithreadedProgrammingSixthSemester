#ifndef PARALLELBFS_TPBFS_HPP
#define PARALLELBFS_TPBFS_HPP

#include <queue>
#include <thread>
#include <unordered_set>
#include <ranges>
#include <RwLock/TRwLock.hpp>
#include <ParallelBFS/TBaseBFS.hpp>

namespace bfs {

template<CBFSUsable T>
class TPBFS : public TBaseBFS<T, TPBFS<T>> {
	friend class TBaseBFS<T, TPBFS<T>>;

	protected:
	TPBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum);

	protected:
	using AVisitorMap = std::unordered_map<T, std::pair<std::atomic_flag, T>>;
	std::optional<AVisitorMap> PredecessorNodesImpl() const;

	protected:
	AVisitorMap CreateVisitorMap() const;

	protected:
	const unsigned m_uThreadsNum = 0;
};

template<CBFSUsable T>
std::unordered_map<T, std::pair<std::atomic_flag, T>> TPBFS<T>::CreateVisitorMap() const {
	auto visitorMap = std::unordered_map<T, std::pair<std::atomic_flag, T>>();
	for(const auto& [key, _] : this->m_refGraph) {
		visitorMap.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple());
	}
	return visitorMap;
}

template<CBFSUsable T>
TPBFS<T>::TPBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum)
	: m_uThreadsNum{std::min(threadsNum, std::jthread::hardware_concurrency())},
	  TBaseBFS<T, TPBFS>(graph, start, end) {}

template<CBFSUsable T>
std::optional<typename TPBFS<T>::AVisitorMap> TPBFS<T>::PredecessorNodesImpl() const {
	auto queue = rwl::TRwLock<std::queue<T>>(std::initializer_list{this->m_refStart});
	auto visitorMap = CreateVisitorMap();
	auto totalEnqueuedNum = std::atomic_size_t{0};
	auto isFoundEndNode = std::atomic_flag{false};
	{
		auto threads = std::vector<std::jthread>();
		for(auto i = 0u; i < m_uThreadsNum; ++i) {
			threads.emplace_back([this, &queue, &visitorMap, &totalEnqueuedNum, &isFoundEndNode]() {
				while(true) {
					if(isFoundEndNode.test(std::memory_order_relaxed)) {
						break;
					}
					if(totalEnqueuedNum.load(std::memory_order_relaxed) >= this->m_refGraph.size()) {
						if(queue.Read()->empty()) {
							break;
						}
					}
					const auto currentNode = [&queue]() -> std::optional<T> {
						auto queueWrite = queue.Write();
						if(queueWrite->empty()) return std::nullopt;
						auto backValue = std::move(queueWrite->front());
						queueWrite->pop();
						return backValue;
					}();
					if(not currentNode) continue;
					for(const auto& neighbour : this->m_refGraph.at(*currentNode)) {
						const auto neighbourIt = visitorMap.find(neighbour);
						if(neighbourIt->second.first.test_and_set(std::memory_order_relaxed)) continue;
						neighbourIt->second.second = *currentNode;
						if(neighbour == this->m_refEnd) {
							isFoundEndNode.test_and_set(std::memory_order_relaxed);
							break;
						}
						totalEnqueuedNum.fetch_add(1, std::memory_order_relaxed);
						queue.Write()->push(neighbour);
					}
				}
			});
		}
	}
	if(not isFoundEndNode.test(std::memory_order_relaxed)) return std::nullopt;
	return visitorMap;
}

}

#endif //PARALLELBFS_TPBFS_HPP
