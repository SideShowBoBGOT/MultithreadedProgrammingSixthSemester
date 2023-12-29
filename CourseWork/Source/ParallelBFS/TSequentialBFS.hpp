#ifndef PARALLELBFS_TSEQUENTIALBFS_HPP
#define PARALLELBFS_TSEQUENTIALBFS_HPP

#include <queue>
#include <unordered_set>
#include <ParallelBFS/TBaseBFS.hpp>

namespace bfs {

template<std::equality_comparable T>
class TSequentialBFS : public TBaseBFS<T, TSequentialBFS<T>> {
	friend class TBaseBFS<T, TSequentialBFS<T>>;
	protected:
	TSequentialBFS(const TGraph<T>& graph, const T& start, const T& end);
	std::unordered_map<T, T> PredecessorNodesImpl() const;
};

template<std::equality_comparable T>
TSequentialBFS<T>::TSequentialBFS(const TGraph<T>& graph, const T& start, const T& end)
	: TBaseBFS<T, TSequentialBFS>(graph, start, end) {}

template<std::equality_comparable T>
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

}

#endif //PARALLELBFS_TSEQUENTIALBFS_HPP
