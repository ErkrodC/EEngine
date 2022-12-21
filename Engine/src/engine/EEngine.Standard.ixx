// ER NOTE this header projection header unit is a temporary while header units
//  (esp std lib ones) are being worked on in msvc + cmake: https://gitlab.kitware.com/cmake/cmake/-/issues/18355
module;
#include <algorithm>
#include <array>
#include <compare>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

export module EEngine.Standard;

export {
	using ::int16_t;
	using ::int32_t;
	using ::int64_t;
	using ::int8_t;
	using ::int_fast16_t;
	using ::int_fast32_t;
	using ::int_fast64_t;
	using ::int_fast8_t;
	using ::int_least16_t;
	using ::int_least32_t;
	using ::int_least64_t;
	using ::int_least8_t;
	using ::intmax_t;
	using ::uint16_t;
	using ::uint32_t;
	using ::uint64_t;
	using ::uint8_t;
	using ::uint_fast16_t;
	using ::uint_fast32_t;
	using ::uint_fast64_t;
	using ::uint_fast8_t;
	using ::uint_least16_t;
	using ::uint_least32_t;
	using ::uint_least64_t;
	using ::uint_least8_t;
	using ::uintmax_t;
}

export namespace std {
	using std::array;
	using std::bind;
	using std::dynamic_pointer_cast;
	using std::find;
	using std::forward;
	using std::function;
	using std::ifstream;
	using std::initializer_list;
	using std::ios;
	using std::make_shared;
	using std::make_unique;
	using std::ostream;
	using std::pair;
	using std::partial_ordering;
	using std::shared_ptr;
	using std::string;
	using std::string_view;
	using std::stringstream;
	using std::unique_ptr;
	using std::unordered_map;
	using std::vector;
	using std::weak_ordering;
}