export module EEngine.Standard;
export import std;

// Re-export common standard types for convenience (global namespace)
export {
	using std::uint8_t;
	using std::uint16_t;
	using std::uint32_t;
	using std::uint64_t;
	using std::int8_t;
	using std::int16_t;
	using std::int32_t;
	using std::int64_t;
	using std::intptr_t;
	using std::size_t;
	using std::float_t;
	using std::double_t;
}

export namespace EEngine {
	template<typename T, typename E>
	using Expected = std::expected<T, E>;

	template<typename E>
	auto Unexpected(E&& error) {
		return std::unexpected<std::decay_t<E>>(std::forward<E>(error));
	}

	template<typename... Args>
	auto Unexpected(std::format_string<Args...> fmt, Args&&... args) {
		return std::unexpected<std::string>(std::format(fmt, std::forward<Args>(args)...));
	}
}