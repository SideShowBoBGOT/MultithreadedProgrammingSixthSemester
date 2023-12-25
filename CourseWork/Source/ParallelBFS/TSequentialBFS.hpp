#ifndef PARALLELBFS_TSEQUENTIALBFS_HPP
#define PARALLELBFS_TSEQUENTIALBFS_HPP

#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <ranges>
#include <concepts>

namespace bfs {

template<std::equality_comparable T>
using TGraph = std::unordered_map<T, std::vector<T>>;

template<std::equality_comparable T>
class TSequentialBFS {
	public:
	static std::vector<T> Do(const TGraph<T>& graph, const T& startNode, const T& endNode) {
		auto visitedNodes = std::vector<T>();
		auto queue = std::queue<T>({startNode});
		auto predecessorNodes = std::unordered_map<T, T>();
		while(not queue.empty()) {
			visitedNodes.push_back(std::move(queue.front()));
			queue.pop();
			const auto& currentNode = visitedNodes.back();
			for(const auto& neighbour : graph.at(currentNode)) {
				if(not std::ranges::contains(visitedNodes, neighbour)) {
					queue.push(neighbour);
					predecessorNodes[neighbour] = currentNode;
				}
			}
		}
		return ShortestPath(predecessorNodes, startNode, endNode);
	}

	protected:
	static std::vector<T> ShortestPath(const std::unordered_map<T, T>& predecessorNodes, const T& startNode, const T& endNode) {
		auto path = std::vector<T>{endNode};
		auto currentNode = endNode;
		while(currentNode != startNode) {
			currentNode = predecessorNodes.at(currentNode);
			path.push_back(currentNode);
		}
		std::reverse(path.begin(), path.end());
		return path;
	}
};

}

#endif //PARALLELBFS_TSEQUENTIALBFS_HPP
