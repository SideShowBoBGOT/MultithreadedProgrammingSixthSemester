#ifndef COURSEWORK_TNODE_H
#define COURSEWORK_TNODE_H

#include <memory>
#include <map>

namespace AStar {
	
	template<typename T>
	concept CAddable = requires(T t) { t + t; };
	
	template<typename T>
	concept CCost = std::is_default_constructible_v<T> && std::three_way_comparable<T> && CAddable<T>;
	
	template<typename DataType, CCost CostType>
	class TNode {
		public:
		TNode() = default;
		explicit TNode(const DataType& data, const std::map<std::weak_ptr<TNode>, CostType>& neighbours);
		
		public:
		const std::map<std::weak_ptr<TNode>, CostType>& Neighbours() const;
		void AddNeighbours(const std::map<std::weak_ptr<TNode>, CostType>& neighbours);
		
		public:
		void Data(const DataType& data);
		const DataType& Data() const;
		DataType& Data();
		
		private:
		DataType m_xData;
		std::map<std::weak_ptr<TNode>, CostType> m_mNeighbours;
	};
	
	template<typename DataType, CCost CostType>
	TNode<DataType, CostType>::TNode(const DataType& data, const std::map<std::weak_ptr<TNode>, CostType>& neighbours)
		: m_xData{data}, m_mNeighbours{neighbours} {
	}
	
	template<typename DataType, CCost CostType>
	void TNode<DataType, CostType>::AddNeighbours(const std::map<std::weak_ptr<TNode>, CostType>& neighbours) {
		std::copy(neighbours.begin(), neighbours.end(), std::back_inserter(m_mNeighbours));
	}
	
	template<typename DataType, CCost CostType>
	const std::map<std::weak_ptr<TNode<DataType, CostType>>, CostType>&
	TNode<DataType, CostType>::Neighbours() const {
		return m_mNeighbours;
	}
	
	template<typename DataType, CCost CostType>
	void TNode<DataType, CostType>::Data(const DataType& data) {
		m_xData = data;
	}
	
	template<typename DataType, CCost CostType>
	const DataType& TNode<DataType, CostType>::Data() const {
		return m_xData;
	}
	
	template<typename DataType, CCost CostType>
	DataType& TNode<DataType, CostType>::Data() {
		return m_xData;
	}
	
}

#endif //COURSEWORK_TNODE_H
