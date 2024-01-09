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

	using ACommunicationResult = std::variant<
		typename NMessage::SEndNodeFound,
		typename NMessage::SAllNodesEnqueued>;

	protected:
	ACommunicationResult Communicate();
	void SendTasks();

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
		TCommunicationCenter(const T& start,
			std::vector<TPipeWriter<AParentMessage>>&& senders,
			std::vector<TPipeReader<AChildrenMessage>>&& listeners);

		public:
		ACommunicationResult Communicate();

		protected:
		void SendTasks();
		ACommunicationResult ListenResults();

		protected:
		TDeque<T> m_vDeque;
		std::vector<TPipeWriter<AParentMessage>> m_vSenders;
		std::vector<TPipeReader<AChildrenMessage>> m_vListeners;
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
	auto communicationCenter = TCommunicationCenter(this->m_refStart, std::move(senders), std::move(listeners));
	const auto communicationResult = communicationCenter.Communicate();
	if(std::holds_alternative<typename NMessage::SEndNodeFound>(communicationResult)) {
		return visitorMap;
	}
	return std::nullopt;
}

template<CBFSUsable T>
TCommunicationBFS<T>::ACommunicationResult TCommunicationBFS<T>::Communicate(
	std::vector<TPipeWriter<AParentMessage>>&& senders,
	std::vector<TPipeReader<AChildrenMessage>>&& listeners) {
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
						senders[i].Write(typename NMessage::SQueueView{begin, ++begin});
					}
				}
				const auto endIt = deque.end();
				for(;i < this->m_uThreadsNum; ++i) {
					senders[i].Write(typename NMessage::SQueueView{endIt, endIt});
				}
			} else {
				const auto step = dequeSize / this->m_uThreadsNum;
				auto begin = deque.begin();
				auto i = std::size_t(0);
				for(; i < this->m_uThreadsNum - 1; ++i) {
					const auto next = begin + step;
					senders[i].Write(typename NMessage::SQueueView{begin, next});
					begin = next;
				}
				senders[i].Write(typename NMessage::SQueueView{begin, deque.end()});
			}
		}
		auto newDeque = TDeque<T>();
		for(auto& l : listeners) {
			auto message = l.Read();
			switch(message.index()) {
				case VariantIndex<AChildrenMessage, typename NMessage::SEndNodeFound>(): {
					for(auto& s : senders) {
						s.Write(typename NMessage::SEndNodeFound{});
					}
					return visitorMap;
				}
				case VariantIndex<AChildrenMessage, typename NMessage::SFrontier>(): {
					auto frontier = std::get<typename NMessage::SFrontier>(message);
					totalEnqueuedNum += frontier.Data.size();
					if(not frontier.Data.empty()) {
						newDeque.push_back(std::move(frontier.Data));
					}
				}
			}
		}
		deque = std::move(newDeque);
		if(totalEnqueuedNum >= this->m_refGraph.size()) {
			for(auto& s : senders) {
				s.Write(typename NMessage::SAllNodesEnqueued{});
			}
			return std::nullopt;
		}
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
	const T& start,
	std::vector<TPipeWriter<AParentMessage>>&& senders,
	std::vector<TPipeReader<AChildrenMessage>>&& listeners)
	: m_vDeque({start}), m_vSenders{std::move(senders)}, m_vListeners{std::move(listeners)} {}

template<CBFSUsable T>
TCommunicationBFS<T>::ACommunicationResult TCommunicationBFS<T>::Communicate() {

}

template<CBFSUsable T>
void TCommunicationBFS<T>::TCommunicationCenter::SendTasks() {

}

template<CBFSUsable T>
TCommunicationBFS<T>::ACommunicationResult TCommunicationBFS<T>::TCommunicationCenter::ListenResults() {

}

}

#endif //PARALLELBFS_TCOMMUNICATIONBFS_HPP
