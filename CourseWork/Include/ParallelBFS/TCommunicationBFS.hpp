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
	std::optional<typename TCommunicationBFS::AVisitorMap> PredecessorNodesImpl() const;

	protected:
	// Messages
	struct SEndNodeFound {};
	struct SAllNodesEnqueued {};
	struct SQueueView {
		const TDeque<T>* Deque;
		size_t Begin;
		size_t End;
	};
	struct SFrontier {
		std::vector<T> Data;
	};

	using AParentMessage = std::variant<
		SEndNodeFound,
		SAllNodesEnqueued,
		SQueueView>;

	using AChildrenMessage = std::variant<
		SEndNodeFound,
		SFrontier>;

	using ACommunicationResult = std::variant<
		SAllNodesEnqueued,
		SEndNodeFound
	>;

	protected:
	AChildrenMessage DoPartialWork(
		const SQueueView& queueView,
		typename TCommunicationBFS::AVisitorMap& visitorMap) const;

	void ChildThreadWork(
		const TPipeWriter<AChildrenMessage>& childSender,
		const TPipeReader<AParentMessage>& parentListener,
		typename TCommunicationBFS::AVisitorMap& visitorMap
	) const;

	AChildrenMessage IterateWork(
		const TDeque<T>& deque,
		const std::vector<TPipeWriter<AParentMessage>>& senders,
		typename TCommunicationBFS::AVisitorMap& visitorMap
	) const;

	auto ProcessPartialResult(
		TDeque<T>& deque,
		AChildrenMessage&& partialResult,
		const std::vector<TPipeWriter<AParentMessage>>& senders,
		size_t& totalEnqueued
	) const -> std::optional<SEndNodeFound>;

	template<typename MessageType>
	void SendMessageToAll(const std::vector<TPipeWriter<AParentMessage>>& senders) const;
};

template<CBFSUsable T>
TCommunicationBFS<T>::TCommunicationBFS(const AGraph<T>& graph, const T& start, const T& end, const unsigned threadsNum)
	: TParallelBFSMixin<T, TCommunicationBFS>(graph, start, end, threadsNum) {}

template<CBFSUsable T>
std::optional<typename TCommunicationBFS<T>::AVisitorMap>
    TCommunicationBFS<T>::PredecessorNodesImpl() const {

	auto deque = TDeque<T>();
	deque.Push({this->m_refStart});
	size_t totalEnqueuedNum = 0;
	auto visitorMap = this->CreateVisitorMap();
	auto senders = std::vector<TPipeWriter<AParentMessage>>();
	auto listeners = std::vector<TPipeReader<AChildrenMessage>>();

	auto threads = std::vector<std::jthread>();
	for(auto i = 0u; i < this->m_uThreadsNum - 1; ++i) {
		auto [parentSender, parentListener] = TPipeChannel<AParentMessage>();
		auto [childrenSender, childrenListener] = TPipeChannel<AChildrenMessage>();
		senders.push_back(std::move(parentSender));
		listeners.push_back(std::move(childrenListener));
		threads.emplace_back([this,
			sender=std::move(childrenSender),
			listener=std::move(parentListener), &visitorMap] {
			ChildThreadWork(sender, listener, visitorMap);
		});
	}

	while(true) {
		auto newDeque = TDeque<T>();
		{
			auto partRes = IterateWork(deque, senders, visitorMap);
			auto endNodeFound = ProcessPartialResult(newDeque,
				std::move(partRes), senders, totalEnqueuedNum);
			if(endNodeFound) {
				for(auto& t : threads)t.join();

				return visitorMap;
			}
		}
		for(auto& l : listeners) {
			auto partRes = l.Read();
			auto endNodeFound = ProcessPartialResult(newDeque,
				std::move(partRes), senders, totalEnqueuedNum);
			if(endNodeFound) {
				for(auto& t : threads)t.join();

				return visitorMap;
			}
		}
		deque = std::move(newDeque);
		if(totalEnqueuedNum >= this->m_refGraph.size()) {
			SendMessageToAll<SAllNodesEnqueued>(senders);
			for(auto& t : threads)t.join();

			return std::nullopt;
		}
	}

	for(auto& t : threads)t.join();

	return std::nullopt;
}

template<CBFSUsable T>
auto TCommunicationBFS<T>::ProcessPartialResult(
	TDeque<T>& deque,
	AChildrenMessage&& partialResult,
	const std::vector<TPipeWriter<AParentMessage>>& senders,
	size_t& totalEnqueued
) const -> std::optional<SEndNodeFound> {
	switch(partialResult.index()) {
		case VariantIndex<AChildrenMessage, SEndNodeFound>(): {
			SendMessageToAll<SEndNodeFound>(senders);
			return SEndNodeFound{};
		}
		case VariantIndex<AChildrenMessage, SFrontier>(): {
			auto& frontier = std::get<SFrontier>(partialResult);
			totalEnqueued += frontier.Data.size();
			if(not frontier.Data.empty()) {
				deque.Push(std::move(frontier.Data));
			}
			return std::nullopt;
		}
		default: {
			lr::UnsopportedCaseError();
		}
	}
}

template<CBFSUsable T>
auto TCommunicationBFS<T>::DoPartialWork(
	const SQueueView& queueView,
	typename TCommunicationBFS::AVisitorMap& visitorMap
) const -> AChildrenMessage {

	auto frontier = SFrontier();
	auto isEndNodeFound = false;
	queueView.Deque->Loop(queueView.Begin, queueView.End,
		[this, &frontier, &isEndNodeFound, &visitorMap](const T& node) {
		for(const auto& neighbour : this->m_refGraph.at(node)) {
			const auto neighbourIt = visitorMap.find(neighbour);
			if(neighbourIt->second.first.test_and_set())
				continue;
			neighbourIt->second.second = node;
			if(neighbour == this->m_refEnd) {
				isEndNodeFound = true;
				return;
			}
			frontier.Data.push_back(neighbour);
		}
	});

	if(isEndNodeFound) return SEndNodeFound{};

	return frontier;
}

template<CBFSUsable T>
void TCommunicationBFS<T>::ChildThreadWork(
	const TPipeWriter<AChildrenMessage>& childSender,
	const TPipeReader<AParentMessage>& parentListener,
	typename TCommunicationBFS::AVisitorMap& visitorMap
) const {
	while(true) {
		const auto parentMessage = parentListener.Read();
		switch(parentMessage.index()) {
			case VariantIndex<AParentMessage, SEndNodeFound>():
			case VariantIndex<AParentMessage, SAllNodesEnqueued>(): {
				return;
			}
			case VariantIndex<AParentMessage, SQueueView>(): {
				const auto& queueView = std::get<SQueueView>(parentMessage);
				childSender.Write(DoPartialWork(queueView, visitorMap));
				break;
			}
			default: {
				lr::UnsopportedCaseError();
			}
		}
	}
}

template<CBFSUsable T>
auto TCommunicationBFS<T>::IterateWork(
	const TDeque<T>& deque,
	const std::vector<TPipeWriter<AParentMessage>>& senders,
	typename TCommunicationBFS::AVisitorMap& visitorMap
) const -> AChildrenMessage {
	const auto dequeSize = deque.Size();
	const auto step = dequeSize / this->m_uThreadsNum;
	const auto remainder = dequeSize % this->m_uThreadsNum;
	for(size_t t = 0, index = 0; t < this->m_uThreadsNum; ++t) {
		auto queueView = SQueueView{};
		queueView.Deque = &deque;
		if(index >= dequeSize) {
			queueView.Begin = dequeSize;
			queueView.End = dequeSize;
		} else {
			const auto localStep = t < remainder ? step + 1 : step;
			queueView.Begin = index;
			queueView.End = index + localStep;
			index += localStep;
		}
		if(t == this->m_uThreadsNum - 1) {
			return DoPartialWork(queueView, visitorMap);
		} else {
			senders[t].Write(queueView);
		}
	}
	return SEndNodeFound{};
}

template<CBFSUsable T>
template<typename MessageType>
void TCommunicationBFS<T>::SendMessageToAll(
	const std::vector<TPipeWriter<AParentMessage>>& senders
) const {
	auto message = MessageType{};
	for(auto& s : senders) {
		s.Write(message);
	}
}

}

#endif //PARALLELBFS_TCOMMUNICATIONBFS_HPP
