//
// Created by choleraplague on 26.06.23.
//

#ifndef COURSEWORK_TASTAR_H
#define COURSEWORK_TASTAR_H

#include <vector>
#include <map>
#include <functional>
#include <memory>

namespace AStar {
	
	template<typename T>
	concept IsCostType = std::is_default_constructible_v<T> && std::three_way_comparable<T> && requires(T t) { t + t; };
	
	template<typename DataType, typename CostType> requires IsCostType<CostType>
	class Node;
	
	template<typename DataType, typename CostType> requires IsCostType<CostType>
	class Node {
		public:
		Node() = default;
		virtual ~Node() = default;
		
		public:
		void AddNeighbours(const std::map<std::shared_ptr<Node>, CostType>& neighbours);
		const std::map<std::shared_ptr<Node>, CostType>& Neighbours() const;
		const std::shared_ptr<DataType>& Data() const;
		
		protected:
		std::shared_ptr<DataType> m_pData = nullptr;
		std::map<std::shared_ptr<Node>, CostType> m_mNeighbours;
	};
	
	template<typename DataType, typename CostType>
	requires IsCostType<CostType>
	void Node<DataType, CostType>::AddNeighbours(
		const std::map<std::shared_ptr<Node>, CostType>& neighbours) {
		std::copy(neighbours.begin(), neighbours.end(), std::back_inserter(m_mNeighbours));
	}
	
	template<typename DataType, typename CostType>
	requires IsCostType<CostType>
	const std::map<std::shared_ptr<Node<DataType, CostType>>, CostType>&
	Node<DataType, CostType>::Neighbours() const {
		return m_mNeighbours;
	}
	
	template<typename DataType, typename CostType>
	requires IsCostType<CostType>
	const std::shared_ptr<DataType>& Node<DataType, CostType>::Data() const {
		return m_pData;
	}
	
	template<typename DataType, typename CostType> requires IsCostType<CostType>
	class TAStar {
		public:
		TAStar() = default;
		virtual~ TAStar() = default;
		
		public:
		using PNode = std::shared_ptr<Node<DataType, CostType>>;
		using PNodes = std::vector<PNode>;
		using Heuristic = std::function<CostType(const PNode&, const PNode&)>;
		
		virtual std::vector<PNode> Solve(const PNode& start, const PNode& end,
										 const PNodes& nodes, const Heuristic& heuristic);
		
		protected:
		
	};
	
	template<typename DataType, typename CostType>
	requires IsCostType<CostType>
	std::vector<typename TAStar<DataType, CostType>::PNode>
	TAStar<DataType, CostType>::Solve(const PNode& start,
									  const PNode& end,
									  const PNodes& nodes,
									  const Heuristic& heuristic) {
		
		auto paths = std::vector<PNodes>();
		paths.push_back({start});
		if(start == end) {
			return paths.front();
		}
		
		auto isStop = false;
		while(!isStop) {
			for(auto& path:paths) {
				const auto& lastNode = path.back();
				
			}
		}
		
	}
}


#endif //COURSEWORK_TASTAR_H
