#ifndef ERROR_HPP
#define ERROR_HPP

#include <chrono>

#define LOG(msg, level) \
	do { \
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); \
		char timestamp[100]; \
		std::strftime(timestamp, sizeof(timestamp), "%Y/%m/%d %H:%M:%S", std::localtime(&now)); \
		std::cout <<"[" << timestamp << "] [" << __FILE__ << "] [" << __LINE__ << "] [" << level << "] [" << msg << "]" << std::endl; \
	} while(0)

#define LOG_INFO(msg) LOG(msg, "INFO")
#define LOG_WARN(msg) LOG(msg, "WARN")
#define LOG_ERROR(msg) LOG(msg, "ERROR")

#define ERROR(msg) \
	do {               \
		LOG_ERROR(msg);  \
		std::terminate();\
	} while(false)

#endif //ERROR_HPP
