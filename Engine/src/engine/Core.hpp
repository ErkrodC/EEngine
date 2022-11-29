#pragma once

#ifdef EE_PLATFORM_WINDOWS

#else
	#error Unsupported platform!
#endif

#ifdef EE_ENABLE_ASSERTS
	#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR(__VA_ARGS__); __debugbreak(); } }
	#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
	#define EE_ASSERT(x, ...)
	#define EE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << (x))
#define BIND_EVENT_FN(x) [this](auto && placeholder0) { return x(std::forward<decltype(placeholder0)>(placeholder0)); }