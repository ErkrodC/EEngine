module;
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h" // ER TODO this goes in core.hpp now?
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
}; // EEngine
