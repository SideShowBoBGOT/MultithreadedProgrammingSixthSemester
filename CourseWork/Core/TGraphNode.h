//
// Created by choleraplague on 28.06.23.
//

#ifndef COURSEWORK_TGRAPHNODE_H
#define COURSEWORK_TGRAPHNODE_H

#include <vector>

#include "TNode.h"

namespace AStar {
	template<typename DataType, CCost CostType>
	class TGraphNode {
		public:
		TGraphNode() = default;
		
		public:
		using UPGraphNode = std::weak_ptr<TGraphNode<DataType, CostType>>;
		using UPChildNode = std::shared_ptr<TGraphNode<DataType, CostType>>;
		
		public:
		explicit TGraphNode(const UPGraphNode& parent);
		
		public:
		const UPGraphNode& Parent() const;
		
		public:
		void AddChildren(const std::vector<UPChildNode>& children);
		const std::vector<UPChildNode>& Children() const;
		
		private:
		UPGraphNode m_pParent = nullptr;
		std::vector<UPChildNode> m_vChildren;
		
	};
	
	template<typename DataType, CCost CostType>
	TGraphNode<DataType, CostType>::TGraphNode(const UPGraphNode& parent) : m_pParent{parent} {
	}
	
	template<typename DataType, CCost CostType>
	void TGraphNode<DataType, CostType>::AddChildren(const std::vector<UPChildNode>& children) {
		std::copy(children.begin(), children.end(), std::back_inserter(m_vChildren));
	}
	
	template<typename DataType, CCost CostType>
	const TGraphNode<DataType, CostType>::UPGraphNode&
	TGraphNode<DataType, CostType>::Parent() const {
		return m_pParent;
	}
	
	template<typename DataType, CCost CostType>
	const std::vector<typename TGraphNode<DataType, CostType>::UPChildNode>&
	TGraphNode<DataType, CostType>::Children() const {
		return m_vChildren;
	}
	
}

#endif //COURSEWORK_TGRAPHNODE_H
