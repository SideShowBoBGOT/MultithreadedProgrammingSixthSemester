#ifndef COURSEWORK_TGRAPHNODE_H
#define COURSEWORK_TGRAPHNODE_H

#include <list>

#include "TNode.h"

namespace AStar {
	template<typename DataType, CCost CostType>
	class TGraphNode {
		public:
		TGraphNode() = default;
		
		public:
		using USPNode = std::shared_ptr<TNode<DataType, CostType>>;
		using UWPGraphNode = std::weak_ptr<TGraphNode<DataType, CostType>>;
		using USPChildNode = std::shared_ptr<TGraphNode<DataType, CostType>>;
		
		public:
		explicit TGraphNode(const UWPGraphNode& parent, const USPNode& node);
		
		public:
		const UWPGraphNode& Parent() const;
		
		public:
		void AddChildren(const std::list<USPChildNode>& children);
		const std::list<USPChildNode>& Children() const;
		
		private:
		USPNode m_pNode = nullptr;
		UWPGraphNode m_pParent = nullptr;
		std::list<USPChildNode> m_vChildren;
	};
	
	template<typename DataType, CCost CostType>
	TGraphNode<DataType, CostType>::TGraphNode(const UWPGraphNode& parent, const USPNode& node)
		: m_pParent{parent}, m_pNode{node} {
	}
	
	template<typename DataType, CCost CostType>
	void TGraphNode<DataType, CostType>::AddChildren(const std::list<USPChildNode>& children) {
		std::copy(children.begin(), children.end(), std::back_inserter(m_vChildren));
	}
	
	template<typename DataType, CCost CostType>
	const TGraphNode<DataType, CostType>::UWPGraphNode&
	TGraphNode<DataType, CostType>::Parent() const {
		return m_pParent;
	}
	
	template<typename DataType, CCost CostType>
	const std::list<typename TGraphNode<DataType, CostType>::USPChildNode>&
	TGraphNode<DataType, CostType>::Children() const {
		return m_vChildren;
	}
	
}

#endif //COURSEWORK_TGRAPHNODE_H
