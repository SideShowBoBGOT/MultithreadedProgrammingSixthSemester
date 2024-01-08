#ifndef PARALLELBFS_TCOMMUNICATIONBFS_HPP
#define PARALLELBFS_TCOMMUNICATIONBFS_HPP

#include <ParallelBFS/THelpers.hpp>
#include <ParallelBFS/TPipes.hpp>
#include <ParallelBFS/TDeque.hpp>
#include <ParallelBFS/TParallelBFSMixin.hpp>

namespace bfs {

template<CBFSUsable T>
class TCommunicationBFS : public TParallelBFSMixin<T, TCommunicationBFS<T>> {
	protected:
	friend class TBaseBFSMixin<T, TCommunicationBFS<T>>;

	protected:
	TCommunicationBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum);

	protected:
	std::optional<typename TCommunicationBFS<T>::AVisitorMap> PredecessorNodesImpl() const;

	protected:
	struct SEndNodeFound {};
	struct SAllNodesEnqueued {};
	struct SQueueView {
		TDequeIterator<T> Begin;
		TDequeIterator<T> End;
	};
	using AParentMessage = std::variant<SEndNodeFound, SAllNodesEnqueued, SQueueView>;
	using AChildrenMessage = std::variant<SEndNodeFound, std::vector<T>>;
};

template<CBFSUsable T>
TCommunicationBFS<T>::TCommunicationBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum)
	: TParallelBFSMixin<T, TCommunicationBFS>(graph, start, end, threadsNum) {}

template<CBFSUsable T>
std::optional<typename TCommunicationBFS<T>::AVisitorMap> TCommunicationBFS<T>::PredecessorNodesImpl() const {
	auto visitorMap = this->CreateVisitorMap();
	auto threads = std::vector<std::jthread>();
	auto senders = std::vector<TPipeWriter<AParentMessage>>();
	auto listeners = std::vector<TPipeReader<AChildrenMessage>>();
	for(auto i = 0u; i < this->m_uThreadsNum; ++i) {
		auto [parentSender, parentListener] = TPipeChannel<AParentMessage>::New();
		auto [childrenSender, childrenListener] = TPipeChannel<AChildrenMessage>::New();
		senders.push_back(std::move(parentSender));
		listeners.push_back(std::move(childrenListener));
		threads.emplace_back([this, &visitorMap, listener=std::move(parentListener),
			sender=std::move(childrenSender)]() {
			while(true) {
				auto parentMessage = listener.Read();
				switch(parentMessage.index()) {
					case VariantIndex<AParentMessage, SEndNodeFound>():
					case VariantIndex<AParentMessage, SAllNodesEnqueued>(): return;
					case VariantIndex<AParentMessage, SQueueView>(): {
						auto frontier = std::vector<T>();
						const auto [begin, end] = std::get<SQueueView>(parentMessage);
						for(auto it = begin; it != end; ++it) {
							const auto& node = *it;
							for(const auto& neighbour : this->m_refGraph.at(node)) {
								const auto neighbourIt = visitorMap.find(neighbour);
								if(neighbourIt->second.first.test_and_set()) continue;
								neighbourIt->second.second = node;
								if(neighbour == this->m_refEnd) {
									sender.Write(SEndNodeFound());
									return;
								}
								frontier.push_back(neighbour);
							}
						}
						sender.Write(std::move(frontier));
					}
				}
			}
		});
	}
	auto totalEnqueuedNum = std::size_t(0);
	auto deque = TDeque<T>();
	deque.push_back({this->m_refStart});
	while(true) {
		{
			const auto dequeSize = deque.size();
			if(dequeSize < this->m_uThreadsNum) {
				auto i = std::size_t(0);
				{
					auto begin = deque.begin();
					for(;i < dequeSize; ++i) {
						senders[i].Write(SQueueView{begin, ++begin});
					}
				}
				const auto endIt = deque.end();
				for(;i < this->m_uThreadsNum; ++i) {
					senders[i].Write(SQueueView{endIt, endIt});
				}
			} else {
				const auto step = dequeSize / this->m_uThreadsNum;
				auto begin = deque.begin();
				auto i = std::size_t(0);
				for(; i < this->m_uThreadsNum - 1; ++i) {
					const auto next = begin + step;
					senders[i].Write(SQueueView{begin, next});
					begin = next;
				}
				senders[i].Write(SQueueView{begin, deque.end()});
			}
		}
		auto newDeque = TDeque<T>();
		for(auto& l : listeners) {
			auto message = l.Read();
			switch(message.index()) {
				case VariantIndex<AChildrenMessage, SEndNodeFound>(): {
					for(auto& s : senders) {
						s.Write(SEndNodeFound());
					}
					return visitorMap;
				}
				case VariantIndex<AChildrenMessage, std::vector<T>>(): {
					auto frontier = std::get<std::vector<T>>(message);
					totalEnqueuedNum += frontier.size();
					if(not frontier.empty()) {
						newDeque.push_back(std::move(frontier));
					}
				}
			}
		}
		deque = std::move(newDeque);
		if(totalEnqueuedNum >= this->m_refGraph.size()) {
			for(auto& s : senders) {
				s.Write(SAllNodesEnqueued());
			}
			return std::nullopt;
		}
	}
	return std::nullopt;
}

}

#endif //PARALLELBFS_TCOMMUNICATIONBFS_HPP
