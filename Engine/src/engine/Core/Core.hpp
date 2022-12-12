#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#ifdef EE_PLATFORM_WINDOWS

#else
	#error Unsupported platform!
#endif

#define EE_CORE_TRACE(...) ::EEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EE_CORE_INFO(...) ::EEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EE_CORE_WARN(...) ::EEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EE_CORE_ERROR(...) ::EEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EE_CORE_FATAL(...) ::EEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client
#define EE_TRACE(...) ::EEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EE_INFO(...) ::EEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define EE_WARN(...) ::EEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EE_ERROR(...) ::EEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define EE_FATAL(...) ::EEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)

#ifdef EE_ENABLE_ASSERTS
	#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR(__VA_ARGS__); __debugbreak(); } }
	#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
#define EE_ASSERT(x, ...)
	#define EE_CORE_ASSERT(x, ...)
#endif

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