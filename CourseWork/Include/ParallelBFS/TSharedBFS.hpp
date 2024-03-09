#ifndef PARALLELBFS_TSHAREDBFS_HPP
#define PARALLELBFS_TSHAREDBFS_HPP

#include <thread>
#include <ParallelBFS/TAtomicQueue.hpp>
#include <ParallelBFS/TParallelBFSMixin.hpp>

namespace bfs {

template<CBFSUsable T>
class TSharedBFS : public TParallelBFSMixin<T, TSharedBFS<T>> {
	protected:
	friend class TBaseBFSMixin<T, TSharedBFS<T>>;

	protected:
	TSharedBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum);

	protected:
	std::optional<typename TSharedBFS<T>::AVisitorMap> PredecessorNodesImpl() const;

	protected:
	class TSharedBFSTask {
		public:
		TSharedBFSTask(
			TAtomicQueue<T>& queue,
			std::unordered_map<T, std::pair<std::atomic_flag, T>>& visitorMap,
			std::atomic_size_t& totalEnqueuedNum,
			std::atomic_flag& isEndNodeFound,
			const AGraph<T>& graph,
			const T& end
		);

		public:
		void operator()();

		protected:
		TAtomicQueue<T>& m_refQueue;
		std::unordered_map<T, std::pair<std::atomic_flag, T>>& m_refVisitorMap;
		std::atomic_size_t& m_refTotalEnqueuedNum;
		std::atomic_flag& m_refIsEndNodeFound;
		const AGraph<T>& m_refGraph;
		const T& m_refEnd;
	};
};

template<CBFSUsable T>
TSharedBFS<T>::TSharedBFSTask::TSharedBFSTask(
	TAtomicQueue<T>& queue,
	std::unordered_map<T, std::pair<std::atomic_flag, T>>& visitorMap,
	std::atomic_size_t& totalEnqueuedNum,
	std::atomic_flag& isEndNodeFound,
	const AGraph<T>& graph,
	const T& end
) : m_refQueue{queue},
	m_refVisitorMap{visitorMap},
	m_refTotalEnqueuedNum{totalEnqueuedNum},
	m_refIsEndNodeFound{isEndNodeFound},
	m_refGraph{graph},
	m_refEnd{end} {}

template<CBFSUsable T>
void TSharedBFS<T>::TSharedBFSTask::operator()() {
	while(not m_refIsEndNodeFound.test()) {
		if(m_refTotalEnqueuedNum.load() >= this->m_refGraph.size()) return;
		const auto currentNodeOpt = m_refQueue.Pop();
		if(not currentNodeOpt) continue;
		const auto& currentNode = currentNodeOpt.value();
		for(const auto& neighbour : this->m_refGraph.at(currentNode)) {
			const auto neighbourIt = m_refVisitorMap.find(neighbour);
			if(neighbourIt->second.first.test_and_set()) continue;
			neighbourIt->second.second = currentNode;
			if(neighbour == this->m_refEnd) {
				m_refIsEndNodeFound.test_and_set();
				return;
			}
			m_refTotalEnqueuedNum.fetch_add(1);
			m_refQueue.Push(neighbour);
		}
	}
}

template<CBFSUsable T>
TSharedBFS<T>::TSharedBFS(const AGraph<T>& graph,
	const T& start, const T& end, const unsigned threadsNum)
	: TParallelBFSMixin<T, TSharedBFS>(graph, start, end, threadsNum) {}

template<CBFSUsable T>
std::optional<typename TSharedBFS<T>::AVisitorMap> TSharedBFS<T>::PredecessorNodesImpl() const {
	auto queue = TAtomicQueue<T>();
	queue.Push(this->m_refStart);
	auto visitorMap = this->CreateVisitorMap();
	auto& isEndNodeFound = visitorMap.find(this->m_refEnd)->second.first;
	{
		auto totalEnqueuedNum = std::atomic_size_t{0};
		auto threads = std::vector<std::jthread>();
		threads.reserve(this->m_uThreadsNum - 1);
		for(auto i = 0u; i < this->m_uThreadsNum - 1; ++i) {
			threads.emplace_back(
				TSharedBFSTask(queue, visitorMap, totalEnqueuedNum,
					isEndNodeFound, this->m_refGraph, this->m_refEnd)
			);
		}
		TSharedBFSTask(queue, visitorMap, totalEnqueuedNum,
			isEndNodeFound, this->m_refGraph, this->m_refEnd)();
	}
	if(not isEndNodeFound.test()) return std::nullopt;
	return visitorMap;
}

}

#endif //PARALLELBFS_TSHAREDBFS_HPP
