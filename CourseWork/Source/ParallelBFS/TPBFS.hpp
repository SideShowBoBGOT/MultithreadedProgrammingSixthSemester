#ifndef PARALLELBFS_TPBFS_HPP
#define PARALLELBFS_TPBFS_HPP

#include <queue>
#include <thread>
#include <unordered_set>
#include <ranges>
#include <RwLock/TRwLock.hpp>
#include <ParallelBFS/TBaseBFS.hpp>

namespace bfs {

template<std::equality_comparable T>
class TPBFS : public TBaseBFS<T, TPBFS<T>> {
	friend class TBaseBFS<T, TPBFS<T>>;

	protected:
	TPBFS(const TGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum);

	protected:
	std::unordered_map<T, std::atomic_flag> CreateVisitorMap() const;
	std::unordered_map<T, T> PredecessorNodesImpl() const;

	protected:
	const unsigned m_uThreadsNum = 0;
};

template<std::equality_comparable T>
std::unordered_map<T, std::atomic_flag> TPBFS<T>::CreateVisitorMap() const {
	auto visitorMap = std::unordered_map<T, std::atomic_flag>();
	for(const auto& key : this->m_refGraph | std::views::keys) {
		visitorMap.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple());
	}
	return visitorMap;
}

template<std::equality_comparable T>
TPBFS<T>::TPBFS(const TGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum)
	: m_uThreadsNum{std::min(threadsNum, std::jthread::hardware_concurrency())},
	  TBaseBFS<T, TPBFS>(graph, start, end) {}

template<std::equality_comparable T>
std::unordered_map<T, T> TPBFS<T>::PredecessorNodesImpl() const {
	const auto visitorMap = CreateVisitorMap();
	auto data = rwl::TRwLock<SynchronizedData>(this->m_refStart);
	{
		auto threads = std::vector<std::jthread>();
		for(auto i = 0u; i < m_uThreadsNum; ++i) {
			threads.emplace_back([this, &data, &visitorMap]() {
				while(true) {
					{
						const auto dataRead = data.Read();
						const auto isEnqueued = dataRead->TotalEnqueuedNum >= this->m_refGraph.size();
						const auto isEmptyQueue = dataRead->Queue.empty();
						if(isEnqueued && isEmptyQueue) {
							break;
						} else if(isEmptyQueue) {
							continue;
						}
					}
					const auto currentNode = [&visitorMap, &data]() -> std::optional<T> {
						auto dataWrite = data.Write();
						if(dataWrite->Queue.empty()) return std::nullopt;
						auto backValue = std::move(dataWrite->Queue.front());
						dataWrite->Queue.pop();
						return backValue;
					}();
					if(not currentNode) continue;
					if(visitorMap.find(currentNode)->second.test_and_set()) continue;
					for(const auto& neighbour : this->m_refGraph.at(*currentNode)) {
						if(visitorMap.contains(neighbour)) continue;
						auto dataWrite = data.Write();
						dataWrite->Queue.push(neighbour);
						dataWrite->PredecessorNodes.insert_or_assign(neighbour, *currentNode);
						dataWrite->TotalEnqueuedNum++;
					}
				}
			});
		}
	}
	return data.Read()->PredecessorNodes;
	return std::unordered_map<T, T>();
}

}

#endif //PARALLELBFS_TPBFS_HPP
