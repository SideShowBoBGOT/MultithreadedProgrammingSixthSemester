#ifndef PARALLELBFS_TCOMMUNICATIONBFS_HPP
#define PARALLELBFS_TCOMMUNICATIONBFS_HPP

#include <cassert>
#include <ParallelBFS/THelpers.hpp>
#include <ParallelBFS/TPipes.hpp>
#include <ParallelBFS/TParallelBFSMixin.hpp>

namespace bfs {

//template<CBFSUsable T>
//class TCommunicationBFS : public TParallelBFSMixin<T, TCommunicationBFS<T>> {
//	protected:
//	friend class TBaseBFSMixin<T, TCommunicationBFS<T>>;
//
//	protected:
//	TCommunicationBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum);
//
//	protected:
//	std::optional<typename TCommunicationBFS<T>::AVisitorMap> PredecessorNodesImpl() const;
//
//	protected:
//	struct SEndNodeFound {};
//	struct SAllNodesEnqueued {};
//	struct SQueueView {};
//	using AParentMessage = std::variant<SEndNodeFound, SAllNodesEnqueued, SQueueView>;
//	using AChildrenMessage = std::variant<SEndNodeFound, std::vector<T>>;
//};
//
//template<CBFSUsable T>
//TCommunicationBFS<T>::TCommunicationBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum)
//	: TParallelBFSMixin<T, TCommunicationBFS>(graph, start, end, threadsNum) {}
//
//template<CBFSUsable T>
//std::optional<typename TCommunicationBFS<T>::AVisitorMap> TCommunicationBFS<T>::PredecessorNodesImpl() const {
//	auto visitorMap = this->CreateVisitorMap();
//	auto totalEnqueuedNum = 0;
//	auto threads = std::vector<std::jthread>();
//	auto senders = std::vector<TPipeWriter<AParentMessage>>();
//	auto listeners = std::vector<TPipeReader<AChildrenMessage>>();
//	for(auto i = 0u; i < this->m_uThreadsNum; ++i) {
//		auto [parentSender, parentListener] = TPipeChannel<AParentMessage>::New();
//		auto [childrenSender, childrenListener] = TPipeChannel<AChildrenMessage>::New();
//		senders.push_back(std::move(parentSender));
//		listeners.push_back(std::move(childrenListener));
//		threads.emplace_back([this, &visitorMap, listener=std::move(parentListener),
//			sender=std::move(childrenSender)]() {
//			while(true) {
//				auto parentMessage = listener.Read();
//				switch(parentMessage.index()) {
//					case VariantIndex<SEndNodeFound>():
//					case VariantIndex<SAllNodesEnqueued>(): return;
//					case VariantIndex<SQueueView>(): {
//						auto frontier = std::vector<T>();
//						for(const auto& node : std::get<SQueueView>()) {
//							for(const auto& neighbour : this->m_refGraph.at(node)) {
//								const auto neighbourIt = visitorMap.find(neighbour);
//								if(neighbourIt->second.first.test_and_set()) continue;
//								neighbourIt->second.second = node;
//								if(neighbour == this->m_refEnd) {
//									sender.Write(SEndNodeFound());
//									return;
//								}
//								frontier.push_back(neighbour);
//							}
//						}
//						sender.Write(std::move(frontier));
//					}
//				}
//			}
//		});
//	}
//	while(true) {
//
//	}
//
//	if(not isEndNodeFound.test()) return std::nullopt;
//	return visitorMap;
//}

}

#endif //PARALLELBFS_TCOMMUNICATIONBFS_HPP
