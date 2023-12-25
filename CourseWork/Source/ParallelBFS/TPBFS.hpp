#ifndef PARALLELBFS_TPBFS_HPP
#define PARALLELBFS_TPBFS_HPP

#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <ranges>
#include <concepts>
#include <thread>
#include <atomic>
#include <RwLock/TRwLock.hpp>

namespace bfs {

template<std::equality_comparable T>
using TGraph = std::unordered_map<T, std::vector<T>>;

template<std::equality_comparable T>
class TPBFS {
	protected:
	struct SynchronizedData {
		size_t TotalEnqueuedNum = 0;
		std::queue<T> Queue;
		std::unordered_map<T, T> PredecessorNodes;
	};

	public:
	static std::vector<T> Do(const TGraph<T>& graph, const T& startNode, const T& endNode, unsigned threadsNum) {
		auto visitedNodes = rwl::TRwLock<std::vector<T>>();
		auto data = rwl::TRwLock<SynchronizedData>();
		data.Write()->Queue.push(startNode);
		threadsNum = std::min(threadsNum, std::jthread::hardware_concurrency());
		{
			auto threads = std::vector<std::jthread>();
			for(auto i = 0u; i < threadsNum; ++i) {
				threads.emplace_back([&data, &visitedNodes, &graph]() {
					const auto isStop = [&data, &graph]() {
						const auto dataRead = data.Read();
						return dataRead->TotalEnqueuedNum == graph.size() && dataRead->Queue.empty();
					}();
					while(!isStop) {
						{
							auto visitedNodesWrite = visitedNodes.Write();
							auto dataWrite = data.Write();
							visitedNodesWrite->push_back(std::move(dataWrite->Queue.front()));
							dataWrite->Queue.pop();
						}
						const auto currentNode = visitedNodes.Read()->back();
						for(const auto& neighbour : graph.at(currentNode)) {
							const auto visitedNodesRead = visitedNodes.Read();
							if(not std::ranges::contains(*visitedNodesRead, neighbour)) {
								auto dataWrite = data.Write();
								dataWrite->Queue.push(neighbour);
								dataWrite->PredecessorNodes.insert_or_assign(neighbour, currentNode);
								dataWrite->TotalEnqueuedNum++;
							}
						}
					}
				});
			}
		}

		return ShortestPath(data.Read()->PredecessorNodes, startNode, endNode);
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

#endif //PARALLELBFS_TPBFS_HPP
