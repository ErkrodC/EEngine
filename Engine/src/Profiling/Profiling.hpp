#pragma once

// Enable profiling by default in Debug builds
#ifndef EE_PROFILING
	#if defined(_DEBUG) || !defined(NDEBUG)
		#define EE_PROFILING 1
	#else
		#define EE_PROFILING 0
	#endif
#endif

#if EE_PROFILING
	// Profiling macros - enabled
	#define EE_PROFILE_SCOPE(name) ::EEngine::Profiling::Timer timer##__LINE__(name)
	#define EE_PROFILE_FUNCTION() EE_PROFILE_SCOPE(__FUNCTION__)
#else
	// Profiling macros - disabled (no overhead in release builds)
	#define EE_PROFILE_SCOPE(name)
	#define EE_PROFILE_FUNCTION()
#endif

// Legacy macro for backwards compatibility
#define PROFILED EE_PROFILE_FUNCTION()
