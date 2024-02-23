#ifndef COMMON_TAGS_HPP
#define COMMON_TAGS_HPP

#include <boost/serialization/utility.hpp>

#include <vector>

namespace mpi_mul {
	class Matrix;

	constexpr auto FROM_MAIN_THREAD_TAG = 1;
	constexpr auto FROM_TASK_THREAD_TAG = 2;

	using IndexedRow = std::pair<unsigned, std::vector<double>>;

	struct Payload {
		unsigned tasks_num = 0;
		std::pair<const Matrix*, const Matrix*> matrices;

		private:
		friend class boost::serialization::access;
		template<class Archive>
			void serialize(Archive& ar, const unsigned int version) {
			ar & tasks_num;
			ar & matrices;
		}
	};
}


#endif //COMMON_TAGS_HPP
