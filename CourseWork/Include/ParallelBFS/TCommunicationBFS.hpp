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
	struct NMessage {
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
	};

	using AParentMessage = std::variant<
		typename NMessage::SEndNodeFound,
		typename NMessage::SAllNodesEnqueued,
		typename NMessage::SQueueView>;

	using AChildrenMessage = std::variant<
		typename NMessage::SEndNodeFound,
		typename NMessage::SFrontier>;

	protected:
	class TCommunicationTask {
		public:
		TCommunicationTask(
			const AGraph<T>& graph,
			const T& end,
			TPipeWriter<AChildrenMessage>&& sender,
			TPipeReader<AParentMessage>&& listener,
			typename TCommunicationBFS<T>::AVisitorMap& visitorMap);

		void operator()();

		protected:
		const AGraph<T>& m_refGraph;
		const T& m_refEnd;
		TPipeWriter<AChildrenMessage> m_xSender;
		TPipeReader<AParentMessage> m_xListener;
		typename TCommunicationBFS<T>::AVisitorMap& m_refVisitorMap;
	};

	protected:
	class TCommunicationCenter {
		public:
		TCommunicationCenter(const size_t graphSize, const T& start,
			std::vector<TPipeWriter<AParentMessage>>&& senders,
			std::vector<TPipeReader<AChildrenMessage>>&& listeners);

		public:
		using ACommunicationResult = std::variant<
			typename NMessage::SEndNodeFound,
			typename NMessage::SAllNodesEnqueued>;

		ACommunicationResult Communicate();

		protected:
		struct SContinueSeeking {};

		using AListenResult = std::variant<
			typename NMessage::SEndNodeFound,
			typename NMessage::SAllNodesEnqueued,
			SContinueSeeking>;

		protected:
		void SendTasks();

		template<typename MessageType>
		MessageType SendMessageToAll();

		AListenResult ListenResults();

		protected:
		const size_t m_uGraphSize;
		TDeque<T> m_vDeque;
		std::vector<TPipeWriter<AParentMessage>> m_vSenders;
		std::vector<TPipeReader<AChildrenMessage>> m_vListeners;
		size_t m_uTotalEnqueuedNum = 0;
	};
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
		auto [parentSender, parentListener] = TPipeChannel<AParentMessage>();
		auto [childrenSender, childrenListener] = TPipeChannel<AChildrenMessage>();
		senders.push_back(std::move(parentSender));
		listeners.push_back(std::move(childrenListener));
		threads.emplace_back(TCommunicationTask(this->m_refGraph, this->m_refEnd,
			std::move(childrenSender), std::move(parentListener), visitorMap));
	}
	auto communicationCenter = TCommunicationCenter(this->m_refGraph.size(), this->m_refStart, std::move(senders), std::move(listeners));
	const auto communicationResult = communicationCenter.Communicate();
	if(std::holds_alternative<typename NMessage::SEndNodeFound>(communicationResult)) {
		return visitorMap;
	}
	return std::nullopt;
}

template<CBFSUsable T>
TCommunicationBFS<T>::TCommunicationTask::TCommunicationTask(
	const AGraph<T>& graph,
	const T& end,
	TPipeWriter<AChildrenMessage>&& sender,
	TPipeReader<AParentMessage>&& listener,
	typename TCommunicationBFS<T>::AVisitorMap& visitorMap)
	: 	m_refGraph{graph},
		m_refEnd{end},
		m_xSender{std::move(sender)},
		m_xListener{std::move(listener)},
		m_refVisitorMap{visitorMap} {}

template<CBFSUsable T>
void TCommunicationBFS<T>::TCommunicationTask::operator()() {
	while(true) {
		auto parentMessage = m_xListener.Read();
		if(not std::holds_alternative<typename NMessage::SQueueView>(parentMessage)) return;
		auto frontier = typename NMessage::SFrontier();
		const auto [deque, begin, end] = std::get<typename NMessage::SQueueView>(parentMessage);
		auto isEndNodeFound = false;
		deque->Loop(begin, end, [this, &frontier, &isEndNodeFound](const T& node) {
			for(const auto& neighbour : this->m_refGraph.at(node)) {
				const auto neighbourIt = m_refVisitorMap.find(neighbour);
				if(neighbourIt->second.first.test_and_set())
					continue;
				neighbourIt->second.second = node;
				if(neighbour == this->m_refEnd) {
					m_xSender.Write(typename NMessage::SEndNodeFound{});
					isEndNodeFound = true;
					return;
				}
				frontier.Data.push_back(neighbour);
			}
		});
		if(isEndNodeFound) return;
		m_xSender.Write(std::move(frontier));
	}
}

template<CBFSUsable T>
TCommunicationBFS<T>::TCommunicationCenter::TCommunicationCenter(
	const size_t graphSize,
	const T& start,
	std::vector<TPipeWriter<AParentMessage>>&& senders,
	std::vector<TPipeReader<AChildrenMessage>>&& listeners)
	: m_uGraphSize{graphSize}, m_vSenders{std::move(senders)}, m_vListeners{std::move(listeners)} {
	m_vDeque.Push({start});
}

template<CBFSUsable T>
TCommunicationBFS<T>::TCommunicationCenter::ACommunicationResult
TCommunicationBFS<T>::TCommunicationCenter::Communicate() {
	while(true) {
		SendTasks();
		const auto result = ListenResults();
		switch(result.index()) {
			case VariantIndex<AListenResult, typename NMessage::SEndNodeFound>(): {
				return typename NMessage::SEndNodeFound();
			}
			case VariantIndex<AListenResult, typename NMessage::SAllNodesEnqueued>(): {
				return typename NMessage::SAllNodesEnqueued();
			}
			case VariantIndex<AListenResult, SContinueSeeking>(): {
				break;
			}
		}
	}
	return typename NMessage::SAllNodesEnqueued{};
}

template<CBFSUsable T>
void TCommunicationBFS<T>::TCommunicationCenter::SendTasks() {
	const auto dequeSize = m_vDeque.Size();
	const auto threadsNum = m_vSenders.size();
	const auto end = m_vDeque.Size();
	auto begin = size_t(0);
	auto i = std::size_t(0);
	const auto step = std::max(size_t(1), dequeSize / threadsNum);
	const auto stepsNum = std::min(dequeSize, threadsNum - 1);
	for(;i < stepsNum; ++i) {
		const auto next = begin + step;
		m_vSenders[i].Write(typename NMessage::SQueueView{&m_vDeque, begin, next});
		begin = next;
	}
	if(stepsNum == dequeSize) {
		for(;i < threadsNum; ++i) {
			m_vSenders[i].Write(typename NMessage::SQueueView{&m_vDeque, end, end});
		}
	} else {
		m_vSenders[i].Write(typename NMessage::SQueueView{&m_vDeque, begin, end});
	}
}

template<CBFSUsable T>
template<typename MessageType>
MessageType TCommunicationBFS<T>::TCommunicationCenter::SendMessageToAll() {
	auto message = MessageType{};
	for(auto& s : m_vSenders) {
		s.Write(message);
	}
	return message;
}

template<CBFSUsable T>
TCommunicationBFS<T>::TCommunicationCenter::AListenResult TCommunicationBFS<T>::TCommunicationCenter::ListenResults() {
	auto newDeque = TDeque<T>();
	for(auto& l : m_vListeners) {
		auto message = l.Read();
		switch(message.index()) {
			case VariantIndex<AChildrenMessage, typename NMessage::SEndNodeFound>(): {
				return SendMessageToAll<typename NMessage::SEndNodeFound>();
			}
			case VariantIndex<AChildrenMessage, typename NMessage::SFrontier>(): {
				auto frontier = std::get<typename NMessage::SFrontier>(message);
				m_uTotalEnqueuedNum += frontier.Data.size();
				if(not frontier.Data.empty()) {
					newDeque.Push(std::move(frontier.Data));
				}
			}
		}
	}
	m_vDeque = std::move(newDeque);
	if(m_uTotalEnqueuedNum >= m_uGraphSize) {
		return SendMessageToAll<typename NMessage::SAllNodesEnqueued>();
	}
	return SContinueSeeking{};
}

}

#endif //PARALLELBFS_TCOMMUNICATIONBFS_HPP
