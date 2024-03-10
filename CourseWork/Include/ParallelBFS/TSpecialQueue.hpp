#ifndef TSPECIALQUEUE_HPP
#define TSPECIALQUEUE_HPP

#include <ParallelBFS/TCursor.hpp>
#include <vector>

namespace bfs {

template<typename T>
class TSpecialQueue {
	public:
	TSpecialQueue(const size_t size)
		: m{}

	public:
	void Push() {

	}

	void Pop() {

	}

	protected:
	std::vector<T> m_vData;
	TCursor m_xPushCursor;
	TCursor m_xPopCursor;
};

}

#endif //TSPECIALQUEUE_HPP
