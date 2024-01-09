#ifndef PARALLELBFS_TDEQUE_HPP
#define PARALLELBFS_TDEQUE_HPP

#include <vector>
#include <optional>

namespace bfs {

template<typename T>
class TDeque {
	public:
	TDeque()=default;

	public:
	void Push(std::vector<T>&& value);
	std::size_t Size() const noexcept;
	void Loop(const size_t begin, const size_t end, const std::function<void(const T& el)>& func) const;

	protected:
	std::vector<std::vector<T>> m_vData;
};

template<typename T>
void TDeque<T>::Push(std::vector<T>&& value) {
	m_vData.push_back(std::move(value));
}

template<typename T>
std::size_t TDeque<T>::Size() const noexcept {
	auto size = size_t(0);
	for(const auto& el : m_vData) {
		size += el.size();
	}
	return size;
}

template<typename T>
void TDeque<T>::Loop(const size_t begin, const size_t end, const std::function<void(const T&)>& func) const {
	if(begin == end) return;
	auto vectorIt = m_vData.begin();
	auto elIt = m_vData.begin()->begin();

	auto delay = begin;
	auto dist = vectorIt->end() - elIt;
	while(delay >= dist) {
		delay -= dist;
		++vectorIt;
		elIt = vectorIt->begin();
		dist = vectorIt->end() - elIt;
	}

	elIt += delay;

	for(auto i = begin; i < end; ++i, ++elIt) {
		if(elIt == vectorIt->end()) {
			++vectorIt;
			elIt = vectorIt->begin();
		}
		func(*elIt);
	}
}

}

#endif //PARALLELBFS_TDEQUE_HPP
