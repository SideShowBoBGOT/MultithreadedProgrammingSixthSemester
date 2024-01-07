#ifndef PARALLELBFS_TCOMMUNICATIONBFS_HPP
#define PARALLELBFS_TCOMMUNICATIONBFS_HPP

#include <ParallelBFS/TParallelBFSMixin.hpp>

namespace bfs {

template<CBFSUsable T>
class TCommunicationBFS : public TParallelBFSMixin<T, TCommunicationBFS<T>> {
	protected:
	friend class TBaseBFSMixin<T, TCommunicationBFS<T>>;

	protected:
	TCommunicationBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum);

	protected:
	std::optional<typename TCommunicationBFS<T>::AVisitorMap> PredecessorNodesImpl() const;
};

template<CBFSUsable T>
TCommunicationBFS<T>::TCommunicationBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum)
	: TParallelBFSMixin<T, TCommunicationBFS>(graph, start, end, threadsNum) {}

template<CBFSUsable T>
std::optional<typename TCommunicationBFS<T>::AVisitorMap> TCommunicationBFS<T>::PredecessorNodesImpl() const {
	auto queue = TAtomicQueue<T>();
	queue.Push(this->m_refStart);
	auto visitorMap = this->CreateVisitorMap();
	auto& isEndNodeFound = visitorMap.find(this->m_refEnd)->second.first;
	auto totalEnqueuedNum = std::atomic_size_t{0};
	{
		auto threads = std::vector<std::jthread>();
		threads.reserve(this->m_uThreadsNum);
		for(auto i = 0u; i < this->m_uThreadsNum; ++i) {
			threads.emplace_back([this, &queue, &visitorMap, &totalEnqueuedNum, &isEndNodeFound]() {
				while(not isEndNodeFound.test()) {
					if(totalEnqueuedNum.load() >= this->m_refGraph.size()) return;
					const auto currentNodeOpt = queue.Pop();
					if(not currentNodeOpt) continue;
					const auto& currentNode = currentNodeOpt.value();
					for(const auto& neighbour : this->m_refGraph.at(currentNode)) {
						const auto neighbourIt = visitorMap.find(neighbour);
						if(neighbourIt->second.first.test_and_set()) continue;
						neighbourIt->second.second = currentNode;
						if(neighbour == this->m_refEnd) return;
						totalEnqueuedNum.fetch_add(1);
						queue.Push(neighbour);
					}
				}
			});
		}
	}
	if(not isEndNodeFound.test()) return std::nullopt;
	return visitorMap;
}

}

#endif //PARALLELBFS_TCOMMUNICATIONBFS_HPP
