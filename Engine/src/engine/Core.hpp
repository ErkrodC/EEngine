#pragma once

#ifdef EE_PLATFORM_WINDOWS
	#ifdef EE_BUILD_DLL
		#define EE_API __declspec(dllexport)
	#else
		#define EE_API __declspec(dllimport)
	#endif
#else
	#error Unsupported platform!
#endif

#ifdef EE_ENABLE_ASSERTS
	#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EE_ASSERT(x, ...)
	#define EE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)