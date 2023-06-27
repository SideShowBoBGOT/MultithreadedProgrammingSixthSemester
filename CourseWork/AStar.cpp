//
// Created by choleraplague on 26.06.23.
//

#include "TAStar.h"
#include <ranges>

TAStar::Nodes TAStar::Solve(const int start, const int end, const Nodes& nodes,
							const Vertexes& vertexes) {
	
	auto paths = std::vector<Nodes>();
	paths.push_back({start});
	
	if(start == end) {
		return paths.front();
	}
	
	auto inspectedNodes = Nodes();
	
	auto isStop = false;
	while(!isStop) {
		for(auto& path:paths) {
			const auto& neighbours = vertexes[path.back()];
			auto vals = std::views::transform(neighbours, []())
		}
	}
	
	
}