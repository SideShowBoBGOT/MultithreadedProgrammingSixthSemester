#ifndef PARALLELBFS_TPBFS_HPP
#define PARALLELBFS_TPBFS_HPP

#include <queue>
#include <thread>
#include <unordered_set>
#include <ranges>
#include <RwLock/TRwLock.hpp>
#include <ParallelBFS/TBaseBFS.hpp>

namespace bfs {

template<typename T>
class TAtomicQueue {
	public:
	TAtomicQueue()=default;

	public:
	template<typename U>
	void Push(U&& value);

	std::optional<T> Pop();
	bool IsEmpty() const;

	protected:
	mutable std::atomic_flag m_xFlag;
	std::queue<T> m_qQueue;
};

template<typename T>
template<typename U>
void TAtomicQueue<T>::Push(U&& value) {
	while(m_xFlag.test_and_set());
	m_qQueue.push(std::forward<U>(value));
	m_xFlag.clear();
}

template<typename T>
std::optional<T> TAtomicQueue<T>::Pop() {
	// Atomic flag should work based on RAII, but I am lazy
	while(m_xFlag.test_and_set());
	if(m_qQueue.empty()) {
		m_xFlag.clear();
		return std::nullopt;
	}
	auto popped = std::move(m_qQueue.front());
	m_qQueue.pop();
	m_xFlag.clear();
	return popped;
}

template<typename T>
bool TAtomicQueue<T>::IsEmpty() const {
	while(m_xFlag.test_and_set());
	const auto res = m_qQueue.empty();
	m_xFlag.clear();
	return res;
}

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
	visitorMap.reserve(this->m_refGraph.size());
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
	auto queue = TAtomicQueue<T>();
	queue.Push(this->m_refStart);
	auto visitorMap = CreateVisitorMap();
	auto totalEnqueuedNum = std::atomic_size_t{0};
	auto isFoundEndNode = std::atomic_flag{false};
	{
		auto threads = std::vector<std::jthread>();
		threads.reserve(m_uThreadsNum);
		for(auto i = 0u; i < m_uThreadsNum; ++i) {
			threads.emplace_back([this, &queue, &visitorMap, &totalEnqueuedNum, &isFoundEndNode]() {
				while(true) {
					if(isFoundEndNode.test(std::memory_order_relaxed)) {
						break;
					}
					if(totalEnqueuedNum.load(std::memory_order_relaxed) >= this->m_refGraph.size()) {
						if(queue.IsEmpty()) {
							break;
						}
					}
					const auto currentNodeOpt = queue.Pop();
					if(not currentNodeOpt) continue;
					const auto& currentNode = currentNodeOpt.value();
					for(const auto& neighbour : this->m_refGraph.at(currentNode)) {
						const auto neighbourIt = visitorMap.find(neighbour);
						if(neighbourIt->second.first.test_and_set(std::memory_order_relaxed)) continue;
						neighbourIt->second.second = currentNode;
						if(neighbour == this->m_refEnd) {
							isFoundEndNode.test_and_set(std::memory_order_relaxed);
							break;
						}
						totalEnqueuedNum.fetch_add(1, std::memory_order_relaxed);
						queue.Push(neighbour);
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
