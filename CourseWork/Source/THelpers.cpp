#include <ParallelBFS/THelpers.hpp>
#include <magic_enum.hpp>
#include <array>
#include <iostream>
#include <chrono>

namespace bfs {

namespace lr {
	void Log(
		const std::string_view message,
		const NLevel level,
		std::source_location location
	) {
		const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		auto timestamp = std::array<char, 100>();
		std::strftime(timestamp.data(), timestamp.size(), "%Y/%m/%d %H:%M:%S", std::localtime(&now));

		std::clog << "[" << magic_enum::enum_name(level) << "] "
			<< "[" << timestamp.data() << "] "
			<< "[" << location.file_name() << "("
			<< location.line() << ":"
			<< location.column() << ")] "
			<< "[" << location.function_name() << "] "
			<< "[" << message << "]\n";
	}

	void LogInfo(
		const std::string_view message,
		std::source_location location
	) {
		Log(message, NLevel::Info, location);
	}

	void LogWarn(
		const std::string_view message,
		std::source_location location
	) {
		Log(message, NLevel::Warn, location);
	}

	void LogError(
		const std::string_view message,
		std::source_location location
	) {
		Log(message, NLevel::Error, location);
	}

	[[noreturn]] void Error(
		const std::string_view message,
		std::source_location location
	) {
		LogError(message, location);
		std::terminate();
	}

	[[noreturn]] void UnsopportedCaseError(
		std::source_location location
	) {
		Error("Unsopported case", location);
	}
}

}