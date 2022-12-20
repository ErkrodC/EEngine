module;
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

export module EEngine.Core:Log;

namespace EEngine::Log {
	static std::shared_ptr<spdlog::logger> s_CoreLogger = nullptr;
	static std::shared_ptr<spdlog::logger> s_ClientLogger = nullptr;

	export void Initialize() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("EENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	export inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	export inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

#if EE_ENABLE_LOGGING
	export template<typename... Args>
	inline void CoreTrace(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->trace(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void CoreInfo(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->info(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void CoreWarn(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->warn(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void CoreError(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->error(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void CoreCritical(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetCoreLogger()->critical(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void CoreAssert(bool value, spdlog::format_string_t<Args...> fmt, Args &&... args) {
		if (!value) {
			CoreError(fmt, std::forward<Args>(args)...);
			__debugbreak();
		}
	}

	export template<typename... Args>
	inline void Trace(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->trace(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void Info(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->info(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void Warn(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->warn(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void Error(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->error(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void Critical(spdlog::format_string_t<Args...> fmt, Args &&... args) {
		GetClientLogger()->critical(fmt, std::forward<Args>(args)...);
	}

	export template<typename... Args>
	inline void Assert(bool value, spdlog::format_string_t<Args...> fmt, Args &&... args) {
		if (!value) {
			Error(fmt, std::forward<Args>(args)...);
			__debugbreak();
		}
	}
#else // EE_ENABLE_LOGGING
	export template<typename... Args>
	inline void CoreTrace(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void CoreInfo(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void CoreWarn(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void CoreError(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void CoreCritical(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void CoreAssert(bool value, spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void Trace(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void Info(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void Warn(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void Error(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void Critical(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

	export template<typename... Args>
	inline void Assert(bool value, spdlog::format_string_t<Args...> fmt, Args &&... args) {}
#endif // EE_ENABLE_LOGGING
}; // EEngine
