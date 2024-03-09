#ifndef PARALLELBFS_THELPERS_HPP
#define PARALLELBFS_THELPERS_HPP

#include <variant>
#include <string_view>
#include <source_location>

namespace bfs {

template<typename VariantType, typename T, std::size_t index = 0>
constexpr std::size_t VariantIndex() {
	static_assert(std::variant_size_v<VariantType> > index, "Type not found in variant");
	if constexpr (index == std::variant_size_v<VariantType>) {
		return index;
	} else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>) {
		return index;
	} else {
		return VariantIndex<VariantType, T, index + 1>();
	}
}

namespace lr {
	enum class NLevel {
		Info,
		Warn,
		Error
	};

	void Log(
		const std::string_view message,
		const NLevel level,
		std::source_location location = std::source_location::current()
	);

	void LogInfo(
		const std::string_view message,
		std::source_location location = std::source_location::current()
	);

	void LogWarn(
		const std::string_view message,
		std::source_location location = std::source_location::current()
	);

	void LogError(
		const std::string_view message,
		std::source_location location = std::source_location::current()
	);

	[[noreturn]] void Error(
		const std::string_view message,
		std::source_location location = std::source_location::current()
	);

	[[noreturn]] void UnsopportedCaseError(
		std::source_location location = std::source_location::current()
	);

}

}

#endif //PARALLELBFS_THELPERS_HPP
