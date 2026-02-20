module;
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

export module EEngine.Core:Log;
import :Memory;

using namespace EEngine;
using namespace spdlog;

// Internal namespace for static storage (not exported)
namespace EEngine::Log::Internal {
	static Shared<logger> s_CoreLogger = nullptr;
	static Shared<logger> s_ClientLogger = nullptr;
}

export namespace EEngine::Log {
	using namespace Internal;

	void Initialize() {
		set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = stdout_color_mt("EENGINE");
		s_CoreLogger->set_level(level::trace);

		s_ClientLogger = stdout_color_mt("APP");
		s_ClientLogger->set_level(level::trace);
	}

	inline Shared<logger>& GetCoreLogger() { return s_CoreLogger; }
	inline Shared<logger>& GetClientLogger() { return s_ClientLogger; }

#if EE_ENABLE_LOGGING
	template<typename... Args>
	inline void CoreTrace(format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->trace(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void CoreInfo(format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->info(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void CoreWarn(format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->warn(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void CoreError(format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->error(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void CoreCritical(format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->critical(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void CoreAssert(bool value, format_string_t<Args...> fmt, Args &&... args) {
		if (!value) {
			CoreError(fmt, std::forward<Args>(args)...);
			__debugbreak();
		}
	}

	template<typename... Args>
	inline void Trace(format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->trace(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void Info(format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->info(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void Warn(format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->warn(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void Error(format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->error(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void Critical(format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->critical(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	inline void Assert(bool value, format_string_t<Args...> fmt, Args &&... args) {
		if (!value) {
			Error(fmt, std::forward<Args>(args)...);
			__debugbreak();
		}
	}
#else
	template<typename... Args>
	inline void CoreTrace(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void CoreInfo(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void CoreWarn(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void CoreError(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void CoreCritical(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void CoreAssert(bool value, format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void Trace(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void Info(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void Warn(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void Error(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void Critical(format_string_t<Args...> fmt, Args &&... args) {}

	template<typename... Args>
	inline void Assert(bool value, format_string_t<Args...> fmt, Args &&... args) {}
#endif
}
