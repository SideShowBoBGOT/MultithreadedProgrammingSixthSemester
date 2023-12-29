#ifndef PARALLELBFS_TSEQUENTIALBFS_HPP
#define PARALLELBFS_TSEQUENTIALBFS_HPP

#include <queue>
#include <unordered_set>
#include <ParallelBFS/TBaseBFS.hpp>

namespace bfs {

template<CBFSUsable T>
class TSequentialBFS : public TBaseBFS<T, TSequentialBFS<T>> {
	friend class TBaseBFS<T, TSequentialBFS<T>>;

	protected:
	TSequentialBFS(const AGraph<T>& graph, const T& start, const T& end);

	protected:
	std::unordered_map<T, T> PredecessorNodesImpl() const;
	static void UpdateCurrentNodeImpl(T& current, const T& value);
};

template<CBFSUsable T>
TSequentialBFS<T>::TSequentialBFS(const AGraph<T>& graph, const T& start, const T& end)
	: TBaseBFS<T, TSequentialBFS>(graph, start, end) {}

template<CBFSUsable T>
std::unordered_map<T, T> TSequentialBFS<T>::PredecessorNodesImpl() const {
	auto visitedNodes = std::unordered_set<T>();
	auto queue = std::queue<T>({this->m_refStart});
	auto predecessorNodes = std::unordered_map<T, T>();
	auto isFoundEndNode = false;
	while(not queue.empty() and not isFoundEndNode) {
		const auto currentNode = std::move(queue.front());
		visitedNodes.insert(currentNode);
		queue.pop();
		for(const auto& neighbour : this->m_refGraph.at(currentNode)) {
			if(not visitedNodes.contains(neighbour)) {
				predecessorNodes[neighbour] = currentNode;
				if(neighbour == this->m_refEnd) {
					isFoundEndNode = true;
					break;
				}
				visitedNodes.insert(neighbour);
				queue.push(neighbour);
			}
		}
	}
	return predecessorNodes;
}

template<CBFSUsable T>
void TSequentialBFS<T>::UpdateCurrentNodeImpl(T& current, const T& value) {
	current = value;
}

}

#endif //PARALLELBFS_TSEQUENTIALBFS_HPP
