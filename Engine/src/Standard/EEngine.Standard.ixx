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