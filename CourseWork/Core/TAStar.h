//
// Created by choleraplague on 26.06.23.
//

#ifndef COURSEWORK_TASTAR_H
#define COURSEWORK_TASTAR_H

#include <vector>
#include <functional>

#include "TNode.h"

namespace AStar {
	
	template<typename DataType, CCost CostType>
	class TAStar {
		public:
		TAStar() = default;
		~TAStar() = default;
		
		public:
		using UPNode = std::weak_ptr<TNode<DataType, CostType>>;
		using UPNodes = std::vector<UPNode>;
		using UHeuristic = std::function<CostType(const UPNode, const UPNode)>;
		using UCostNode = std::pair<UPNode, CostType>;
		
		std::vector<UPNode> Solve(const UPNode& start, const UPNode& end,
								  const UPNodes& TNodes, const UHeuristic& heuristic);
		
		protected:
		
	};
	
	template<typename DataType, CCost CostType>
	std::vector<typename TAStar<DataType, CostType>::UPNode>
	TAStar<DataType, CostType>::Solve(const UPNode& start,
									  const UPNode& end,
									  const UPNodes& TNodes,
									  const UHeuristic& heuristic) {
		
		auto paths = std::vector<UPNodes>();
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
