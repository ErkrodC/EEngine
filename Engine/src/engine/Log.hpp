#pragma once

#include "Core.hpp"
#include "spdlog/spdlog.h"

namespace EEngine {
	class Log {
		public:
			static void Init();
			inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
			inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		private:
			static std::shared_ptr<spdlog::logger> s_CoreLogger;
			static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
} // EEngine

// Core
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
