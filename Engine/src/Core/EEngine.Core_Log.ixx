module;
#include "Core.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

export module EEngine.Core:Log;
import :Memory;

using namespace EEngine;
using namespace spdlog;

namespace EEngine::Log {
	inline Shared<logger>& GetCoreLogger() {
		static Shared<logger> s_CoreLogger = [] {
			auto logger = stdout_color_mt("EENGINE");
			logger->set_pattern("%^[%T] %n: %v%$");
			logger->set_level(level::trace);
			return logger;
		}();

		return s_CoreLogger;
	}

	inline Shared<logger>& GetClientLogger() {
		static Shared<logger> s_ClientLogger = [] {
			auto logger = stdout_color_mt("APP");
			logger->set_pattern("%^[%T] %n: %v%$");
			logger->set_level(level::trace);
			return logger;
		}();

		return s_ClientLogger;
	}

	export {
#if EE_ENABLE_LOGGING
		template<typename... Args>
		void CoreTrace(format_string_t<Args...> fmt, Args &&... args) {
			GetCoreLogger()->trace(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreInfo(format_string_t<Args...> fmt, Args &&... args) {
			GetCoreLogger()->info(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreWarn(format_string_t<Args...> fmt, Args &&... args) {
			GetCoreLogger()->warn(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreError(format_string_t<Args...> fmt, Args &&... args) {
			GetCoreLogger()->error(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreCritical(format_string_t<Args...> fmt, Args &&... args) {
			GetCoreLogger()->critical(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void CoreAssert(bool value, format_string_t<Args...> fmt, Args &&... args) {
			if (!value) {
				CoreError(fmt, std::forward<Args>(args)...);
				DEBUGBREAK();
			}
		}

		template<typename... Args>
		void Trace(format_string_t<Args...> fmt, Args &&... args) {
			GetClientLogger()->trace(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Info(format_string_t<Args...> fmt, Args &&... args) {
			GetClientLogger()->info(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Warn(format_string_t<Args...> fmt, Args &&... args) {
			GetClientLogger()->warn(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Error(format_string_t<Args...> fmt, Args &&... args) {
			GetClientLogger()->error(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Critical(format_string_t<Args...> fmt, Args &&... args) {
			GetClientLogger()->critical(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		void Assert(bool value, format_string_t<Args...> fmt, Args &&... args) {
			if (!value) {
				Error(fmt, std::forward<Args>(args)...);
				DEBUGBREAK();
			}
		}
#else
		template<typename... Args>
		void CoreTrace(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void CoreInfo(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void CoreWarn(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void CoreError(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void CoreCritical(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void CoreAssert(bool value, format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void Trace(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void Info(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void Warn(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void Error(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void Critical(format_string_t<Args...> fmt, Args &&... args) {}

		template<typename... Args>
		void Assert(bool value, format_string_t<Args...> fmt, Args &&... args) {}
#endif
	}
}
