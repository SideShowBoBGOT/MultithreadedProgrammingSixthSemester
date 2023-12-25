#ifndef PARALLELBFS_TSEQUENTIALBFS_HPP
#define PARALLELBFS_TSEQUENTIALBFS_HPP

#include <queue>
#include <ParallelBFS/TBaseBFS.hpp>

namespace bfs {

template<std::equality_comparable T>
class TSequentialBFS : public TBaseBFS<T, TSequentialBFS<T>> {
	protected:
	TSequentialBFS(const TGraph<T>& graph, const T& start, const T& end);
	std::unordered_map<T, T> PredecessorNodesImpl() const;
};

template<std::equality_comparable T>
TSequentialBFS<T>::TSequentialBFS(const TGraph<T>& graph, const T& start, const T& end)
	: TBaseBFS<T, TSequentialBFS>(graph, start, end) {}

template<std::equality_comparable T>
std::unordered_map<T, T> TSequentialBFS<T>::PredecessorNodesImpl() const {
	auto visitedNodes = std::vector<T>();
	auto queue = std::queue<T>({this->m_refStart});
	auto predecessorNodes = std::unordered_map<T, T>();
	while(not queue.empty()) {
		visitedNodes.push_back(std::move(queue.front()));
		queue.pop();
		const auto& currentNode = visitedNodes.back();
		for(const auto& neighbour : this->m_refGraph.at(currentNode)) {
			if(not std::ranges::contains(visitedNodes, neighbour)) {
				queue.push(neighbour);
				predecessorNodes[neighbour] = currentNode;
			}
		}
	}
	return predecessorNodes;
}

}

#endif //PARALLELBFS_TSEQUENTIALBFS_HPP
