#ifndef PARALLELBFS_TDEQUE_HPP
#define PARALLELBFS_TDEQUE_HPP

#include <vector>
#include <optional>

namespace bfs {

template<typename T>
class TDequeIterator {
	public:
	TDequeIterator(	std::vector<std::vector<T>>::iterator outerEnd,
					std::vector<std::vector<T>>::iterator outer,
					std::optional<typename std::vector<T>::iterator> inner);

	public:
	TDequeIterator& operator++();
	TDequeIterator operator++(int);
	T const*& operator->() const noexcept;
	T& operator*() const noexcept;
	auto operator<=>(const TDequeIterator&) const = default;

	protected:
	std::vector<std::vector<T>>::iterator m_xOuterEnd;
	std::vector<std::vector<T>>::iterator m_xOuter;
	std::optional<typename std::vector<T>::iterator> m_xInner;
};

template<typename T>
TDequeIterator<T>::TDequeIterator(
	std::vector<std::vector<T>>::iterator outerEnd,
	std::vector<std::vector<T>>::iterator outer,
	std::optional<typename std::vector<T>::iterator> inner)
	: 	m_xOuterEnd{outerEnd},
		m_xOuter{outer},
		m_xInner{inner} {}

template<typename T>
TDequeIterator<T>& TDequeIterator<T>::operator++() {
	const auto isReachedLocalEnd = [this]() {
		auto& inner = m_xInner.value();
		++inner;
		return inner == m_xOuter->end();
	}();
	if(isReachedLocalEnd) {
		++m_xOuter;
		if(m_xOuter == m_xOuterEnd) {
			m_xInner.reset();
		} else {
			m_xInner = m_xOuter->begin();
		}
	}
	return *this;
}

template<typename T>
TDequeIterator<T> TDequeIterator<T>::operator++(int) {
	auto iterator = *this;
	++(*this);
	return iterator;
}

template<typename T>
T const*& TDequeIterator<T>::operator->() const noexcept {
	return m_xInner.base();
}

template<typename T>
T& TDequeIterator<T>::operator*() const noexcept {
	return m_xInner->operator*();
}

template<typename T>
class TDeque {
	public:
	TDeque()=default;

	// STL-compatible interface
	public:
	void push_back(const std::vector<T>& value);
	void push_back(std::vector<T>&& value);

	public:
	TDequeIterator<T> begin();
	TDequeIterator<T> end();

	protected:
	std::vector<std::vector<T>> m_vData;
};

template<typename T>
void TDeque<T>::push_back(const std::vector<T>& value) {
	m_vData.push_back(value);
}

template<typename T>
void TDeque<T>::push_back(std::vector<T>&& value) {
	m_vData.push_back(std::move(value));
}

template<typename T>
TDequeIterator<T> TDeque<T>::begin() {
	const auto beginIt = m_vData.begin();
	const auto endIt = m_vData.end();
	if(beginIt == endIt) {
		return {endIt, beginIt, std::nullopt};
	}
	return {endIt, beginIt, beginIt->begin()};
}

template<typename T>
TDequeIterator<T> TDeque<T>::end() {
	const auto endIt = m_vData.end();
	return {endIt, endIt, std::nullopt};
}

}

#endif //PARALLELBFS_TDEQUE_HPP
