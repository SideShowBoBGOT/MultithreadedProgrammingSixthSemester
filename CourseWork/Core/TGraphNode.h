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
		using UPGraphNode = std::weak_ptr<TGraphNode<DataType, CostType>>;
		using UPChildNode = std::shared_ptr<TGraphNode<DataType, CostType>>;
		
		public:
		explicit TGraphNode(const UPGraphNode& parent);
		
		public:
		const UPGraphNode& Parent() const;
		
		public:
		void AddChildren(const std::list<UPChildNode>& children);
		const std::list<UPChildNode>& Children() const;
		
		private:
		UPGraphNode m_pParent = nullptr;
		std::list<UPChildNode> m_vChildren;
		
	};
	
	template<typename DataType, CCost CostType>
	TGraphNode<DataType, CostType>::TGraphNode(const UPGraphNode& parent) : m_pParent{parent} {
	}
	
	template<typename DataType, CCost CostType>
	void TGraphNode<DataType, CostType>::AddChildren(const std::list<UPChildNode>& children) {
		std::copy(children.begin(), children.end(), std::back_inserter(m_vChildren));
	}
	
	template<typename DataType, CCost CostType>
	const TGraphNode<DataType, CostType>::UPGraphNode&
	TGraphNode<DataType, CostType>::Parent() const {
		return m_pParent;
	}
	
	template<typename DataType, CCost CostType>
	const std::list<typename TGraphNode<DataType, CostType>::UPChildNode>&
	TGraphNode<DataType, CostType>::Children() const {
		return m_vChildren;
	}
	
}

#endif //COURSEWORK_TGRAPHNODE_H
