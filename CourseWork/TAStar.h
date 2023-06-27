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
	concept IsCostType = std::three_way_comparable<T> && requires(T t) { t + t; };
	
	template<typename DataType, typename CostType> requires IsCostType<CostType>
	class Node;
	
	template<typename DataType, typename CostType> requires IsCostType<CostType>
	using PNode = std::shared_ptr<Node<DataType, CostType>>;
	
	template<typename DataType, typename CostType> requires IsCostType<CostType>
	class Node {
		public:
		Node() = default;
		virtual ~Node() = default;
		
		public:
		void AddNeighbours(const std::map<PNode<DataType, CostType>, CostType>& neighbours);
		const std::shared_ptr<DataType>& Data() const;
		
		protected:
		std::shared_ptr<DataType> m_pData = nullptr;
		std::map<PNode<DataType, CostType>, CostType> m_mNeighbours;
	};
	
	template<typename DataType, typename CostType>
	requires IsCostType<CostType>
	void Node<DataType, CostType>::AddNeighbours(
		const std::map<PNode<DataType, CostType>, CostType>& neighbours) {
		std::copy(neighbours.begin(), neighbours.end(), std::back_inserter(m_mNeighbours));
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
		using Heuristic = std::function<CostType(const std::shared_ptr<Node<DataType, CostType>>&,
												 const std::shared_ptr<Node<DataType, CostType>>&)>;
		
		virtual std::vector<PNode<DataType, CostType>> Solve(const PNode<DataType, CostType>& start,
															 const PNode<DataType, CostType>& end,
															 const std::vector<PNode<DataType, CostType>>& nodes,
															 const Heuristic& heuristic) const;
	};
	
	template<typename DataType, typename CostType>
	requires IsCostType<CostType>
	std::vector<PNode<DataType, CostType>> TAStar<DataType, CostType>::Solve(const PNode<DataType, CostType>& start,
																			 const PNode<DataType, CostType>& end,
																			 const std::vector<PNode<DataType, CostType>>& nodes,
																			 const Heuristic& heuristic) const {
		
		
	}
}


#endif //COURSEWORK_TASTAR_H
