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
	using AVisitorMap = std::unordered_map<T, std::pair<bool, T>>;
	std::optional<AVisitorMap> PredecessorNodesImpl() const;

	protected:
	AVisitorMap CreateVisitorMap() const;
};

template<CBFSUsable T>
TSequentialBFS<T>::TSequentialBFS(const AGraph<T>& graph, const T& start, const T& end)
	: TBaseBFS<T, TSequentialBFS>(graph, start, end) {}

template<CBFSUsable T>
std::optional<typename TSequentialBFS<T>::AVisitorMap> TSequentialBFS<T>::PredecessorNodesImpl() const {
	auto queue = std::queue<T>({this->m_refStart});
	auto visitorMap = CreateVisitorMap();
	auto isFoundEndNode = false;
	while(not queue.empty() and not isFoundEndNode) {
		const auto currentNode = std::move(queue.front());
		queue.pop();
		for(const auto& neighbour : this->m_refGraph.at(currentNode)) {
			const auto neighbourIt = visitorMap.find(neighbour);
			if(not neighbourIt->second.first) {
				neighbourIt->second.first = true;
				neighbourIt->second.second = currentNode;
				if(neighbour == this->m_refEnd) {
					isFoundEndNode = true;
					break;
				}
				queue.push(neighbour);
			}
		}
	}
	if(not isFoundEndNode) return std::nullopt;
	return visitorMap;
}

template<CBFSUsable T>
TSequentialBFS<T>::AVisitorMap TSequentialBFS<T>::CreateVisitorMap() const {
	auto visitorMap = std::unordered_map<T, std::pair<bool, T>>();
	visitorMap.reserve(this->m_refGraph.size());
	for(const auto& [key, _] : this->m_refGraph) {
		visitorMap.insert_or_assign(key, std::make_pair(false, T()));
	}
	return visitorMap;
}

}

#endif //PARALLELBFS_TSEQUENTIALBFS_HPP
