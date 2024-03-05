#ifndef PARALLELBFS_TATOMICQUEUE_HPP
#define PARALLELBFS_TATOMICQUEUE_HPP

#include <optional>
#include <atomic>
#include <queue>

namespace bfs {

template<typename T>
class TAtomicQueue {
	public:
	TAtomicQueue()=default;

	public:
	template<typename U>
	void Push(U&& value);
	std::optional<T> Pop();

	protected:
	std::mutex m_xMutex;
	std::queue<T> m_qQueue;
};

template<typename T>
template<typename U>
void TAtomicQueue<T>::Push(U&& value) {
	const auto lock = std::lock_guard(m_xMutex);
	m_qQueue.push(std::forward<U>(value));
}

template<typename T>
std::optional<T> TAtomicQueue<T>::Pop() {
	const auto lock = std::lock_guard(m_xMutex);
	auto popped = std::move(m_qQueue.front());
	m_qQueue.pop();
	return popped;
}

}

#endif //PARALLELBFS_TATOMICQUEUE_HPP
