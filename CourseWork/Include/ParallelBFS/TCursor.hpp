#ifndef TCURSOR_HPP
#define TCURSOR_HPP

#include <atomic>

namespace bfs {

	class TCursor {
		public:
		explicit TCursor(const size_t size);

		public:
		size_t GetIncrement();

		protected:
		const size_t m_uSize = 0;
		std::atomic_size_t m_uValue = 0;
	};

}

#endif //TCURSOR_HPP
