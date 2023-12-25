#ifndef PARALLELBFS_TPBFS_HPP
#define PARALLELBFS_TPBFS_HPP

#include <queue>
#include <thread>
#include <unordered_set>
#include <RwLock/TRwLock.hpp>
#include <ParallelBFS/TBaseBFS.hpp>

namespace bfs {

template<std::equality_comparable T>
class TPBFS : public TBaseBFS<T, TPBFS<T>> {
	friend class TBaseBFS<T, TPBFS<T>>;

	protected:
	TPBFS(const TGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum);

	protected:
	struct SynchronizedData {
		SynchronizedData(const T& queueVal);
		size_t TotalEnqueuedNum = 0;
		std::queue<T> Queue;
		std::unordered_map<T, T> PredecessorNodes;
	};

	protected:
	std::unordered_map<T, T> PredecessorNodesImpl() const;

	protected:
	const unsigned m_uThreadsNum = 0;
};

template<std::equality_comparable T>
TPBFS<T>::TPBFS(const TGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum)
	: m_uThreadsNum{std::min(threadsNum, std::jthread::hardware_concurrency())},
	  TBaseBFS<T, TPBFS>(graph, start, end) {}

template<std::equality_comparable T>
TPBFS<T>::SynchronizedData::SynchronizedData(const T& queueVal)
	: Queue({queueVal}) {}

template<std::equality_comparable T>
std::unordered_map<T, T> TPBFS<T>::PredecessorNodesImpl() const {
	auto visitedNodes = rwl::TRwLock<std::unordered_set<T>>();
	auto data = rwl::TRwLock<SynchronizedData>(this->m_refStart);
	{
		auto threads = std::vector<std::jthread>();
		for(auto i = 0u; i < m_uThreadsNum; ++i) {
			threads.emplace_back([this, &data, &visitedNodes]() {
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
					const auto currentNode = [&visitedNodes, &data]() -> std::optional<T> {
						auto visitedNodesWrite = visitedNodes.Write();
						auto dataWrite = data.Write();
						if(dataWrite->Queue.empty()) return std::nullopt;
						auto backValue = std::move(dataWrite->Queue.front());
						const auto isInserted = visitedNodesWrite->insert(backValue).second;
						dataWrite->Queue.pop();
						if(not isInserted) return std::nullopt;
						return backValue;
					}();
					if(not currentNode) continue;
					for(const auto& neighbour : this->m_refGraph.at(*currentNode)) {
						if(visitedNodes.Read()->contains(neighbour)) continue;
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
}

}

#endif //PARALLELBFS_TPBFS_HPP
