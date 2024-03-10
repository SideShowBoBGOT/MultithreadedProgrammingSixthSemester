#include <ParallelBFS/TCursor.hpp>

namespace bfs {

TCursor::TCursor(const size_t size)
	: m_uSize{size} {}

size_t TCursor::GetIncrement() {
	while(true) {
		const auto val = m_uValue++;
		if(val < m_uSize) {
			return val;
		}
		auto expected = val;
		do {
			m_uValue.compare_exchange_weak(expected, 0);
		} while(expected >= m_uSize);
	}
}

}