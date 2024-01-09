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
			TDequeIterator<T> Begin;
			TDequeIterator<T> End;
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
		auto [parentSender, parentListener] = TPipeChannel<AParentMessage>::New();
		auto [childrenSender, childrenListener] = TPipeChannel<AChildrenMessage>::New();
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
		const auto [begin, end] = std::get<typename NMessage::SQueueView>(parentMessage);
		for(auto it = begin; it != end; ++it) {
			const auto& node = *it;
			for(const auto& neighbour : this->m_refGraph.at(node)) {
				const auto neighbourIt = m_refVisitorMap.find(neighbour);
				if(neighbourIt->second.first.test_and_set()) continue;
				neighbourIt->second.second = node;
				if(neighbour == this->m_refEnd) {
					m_xSender.Write(typename NMessage::SEndNodeFound{});
					return;
				}
				frontier.Data.push_back(neighbour);
			}
		}
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
		m_vDeque.push_back({start});
	}

template<CBFSUsable T>
TCommunicationBFS<T>::TCommunicationCenter::ACommunicationResult
TCommunicationBFS<T>::TCommunicationCenter::Communicate() {
	while(true) {
		SendTasks();
		const auto result = ListenResults();
		switch(result.index()) {
			case VariantIndex<AListenResult, typename NMessage::SEndNodeFound>(): {
				return std::get<typename NMessage::SEndNodeFound>(result);
			}
			case VariantIndex<AListenResult, typename NMessage::SAllNodesEnqueued>(): {
				return std::get<typename NMessage::SAllNodesEnqueued>(result);
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
	const auto dequeSize = m_vDeque.size();
	const auto threadsNum = m_vSenders.size();
	if(dequeSize < threadsNum) {
		auto i = std::size_t(0);
		{
			auto begin = m_vDeque.begin();
			for(;i < dequeSize; ++i) {
				m_vSenders[i].Write(typename NMessage::SQueueView{begin, ++begin});
			}
		}
		const auto endIt = m_vDeque.end();
		for(;i < threadsNum; ++i) {
			m_vSenders[i].Write(typename NMessage::SQueueView{endIt, endIt});
		}
	} else {
		const auto step = dequeSize / threadsNum;
		auto begin = m_vDeque.begin();
		auto i = std::size_t(0);
		for(; i < threadsNum - 1; ++i) {
			const auto next = begin + step;
			m_vSenders[i].Write(typename NMessage::SQueueView{begin, next});
			begin = next;
		}
		m_vSenders[i].Write(typename NMessage::SQueueView{begin, m_vDeque.end()});
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
					newDeque.push_back(std::move(frontier.Data));
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
