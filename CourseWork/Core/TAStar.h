#ifndef COURSEWORK_TASTAR_H
#define COURSEWORK_TASTAR_H

#include <list>
#include <functional>

#include "TNode.h"

namespace AStar {
	
	template<typename DataType, CCost CostType>
	class TAStar {
		public:
		TAStar() = default;
		
		public:
		using UWPNode = std::weak_ptr<TNode<DataType, CostType>>;
		using UPNodes = std::list<UWPNode>;
		using UHeuristic = std::function<CostType(const UWPNode, const UWPNode)>;
		using UCostNode = std::pair<UWPNode, CostType>;
		
		std::list<UWPNode> Solve(const UWPNode& start, const UWPNode& end,
								 const UPNodes& TNodes, const UHeuristic& heuristic);
		
		private:
		
	};
	
	template<typename DataType, CCost CostType>
	std::list<typename TAStar<DataType, CostType>::UWPNode>
	TAStar<DataType, CostType>::Solve(const UWPNode& start,
									  const UWPNode& end,
									  const UPNodes& TNodes,
									  const UHeuristic& heuristic) {
		
		paths.push_back({start});
		if(start == end) {
			return paths.front();
		}
		
		auto isStop = false;
		while(!isStop) {
			for(auto& path:paths) {
				const auto& lastTNode = path.back();
				
			}
		}
		
	}
}

#endif //COURSEWORK_TASTAR_H
