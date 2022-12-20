#pragma once

// ER NOTE still required where logging is used, spdlog says "support for user defined types"
// can't seem to get rid of until compiler supports forwarding operator overloads from header-wrapping modules
// i.e. EEngine.Standard.ixx
#include "spdlog/fmt/ostr.h"

#define BIT(x) (1 << (x))
#define BIND_EVENT_FN(x) [this](auto && placeholder0) { return x(std::forward<decltype(placeholder0)>(placeholder0)); }

// cross-compiler warning disable macros
#if defined(_MSC_VER)
	#define DISABLE_WARNING_PUSH           __pragma(warning( push ))
	#define DISABLE_WARNING_POP            __pragma(warning( pop ))
	#define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))

	#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(4100)
	#define DISABLE_WARNING_UNREFERENCED_FUNCTION            DISABLE_WARNING(4505)
	#define DISABLE_WARNING_NAMELESS_STRUCT_UNION            DISABLE_WARNING(4201)
// other warnings you want to deactivate...

#elif defined(__GNUC__) || defined(__clang__)
#define DO_PRAGMA(X) _Pragma(#X)
    #define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
    #define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop)
    #define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)

	#define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    DISABLE_WARNING(-Wunused-parameter)
	#define DISABLE_WARNING_UNREFERENCED_FUNCTION            DISABLE_WARNING(-Wunused-function)

	// differing warning names...
	#if __GNUC__
		#define DISABLE_WARNING_NAMELESS_STRUCT_UNION             DISABLE_WARNING(-Wno-gnu-anonymous-struct)
	#else
		#define DISABLE_WARNING_NAMELESS_STRUCT_UNION            DISABLE_WARNING(-Wno-pedantic)
	#endif
   // other warnings you want to deactivate...

#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
    #define DISABLE_WARNING_UNREFERENCED_FUNCTION
    // other warnings you want to deactivate...

#endif