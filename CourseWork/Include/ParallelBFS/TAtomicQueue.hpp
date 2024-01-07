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
	mutable std::atomic_flag m_xFlag;
	std::queue<T> m_qQueue;
};

template<typename T>
template<typename U>
void TAtomicQueue<T>::Push(U&& value) {
	while(m_xFlag.test_and_set());
	m_qQueue.push(std::forward<U>(value));
	m_xFlag.clear();
}

template<typename T>
std::optional<T> TAtomicQueue<T>::Pop() {
	// Atomic flag should work based on RAII, but I am lazy
	while(m_xFlag.test_and_set());
	if(m_qQueue.empty()) {
		m_xFlag.clear();
		return std::nullopt;
	}
	auto popped = std::move(m_qQueue.front());
	m_qQueue.pop();
	m_xFlag.clear();
	return popped;
}

}

#endif //PARALLELBFS_TATOMICQUEUE_HPP
