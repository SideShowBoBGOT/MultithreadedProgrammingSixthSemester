//
// Created by choleraplague on 26.06.23.
//

#ifndef COURSEWORK_TASTAR_H
#define COURSEWORK_TASTAR_H

#include <vector>
#include <map>
#include <functional>
#include <memory>

template<typename T>
concept IsCostType = std::three_way_comparable<T> && requires(T t) { t + t; };

template<typename DataType, typename CostType> requires IsCostType<CostType>
class Node {
	public:
	Node() = default;
	virtual ~Node() = default;
	
	public:
	void AddNeighbours(const std::map<std::shared_ptr<Node>, CostType>& neighbours);
	const std::shared_ptr<DataType>& Data() const;
	
	protected:
	std::shared_ptr<DataType> m_pData = nullptr;
	std::map<std::shared_ptr<Node>, CostType> m_mNeighbours;
};

template<typename DataType, typename CostType>
requires IsCostType<CostType>
void Node<DataType, CostType>::AddNeighbours(
	const std::map<std::shared_ptr<Node<DataType, CostType>>, CostType>& neighbours) {
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
	using Nodes = std::vector<int>;
	using Vertexes = std::vector<std::vector<std::pair<int, double>>>;
	virtual Nodes Solve(const int start, const int end,
						const Nodes& nodes, const Vertexes& vertexes);
	
};


#endif //COURSEWORK_TASTAR_H
