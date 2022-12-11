module;
#include "spdlog/spdlog.h"
//#include "spdlog/fmt/ostr.h" // ER TODO this goes in core.hpp now?

export module EEngine.Core:Log;

export namespace EEngine {
	class Log {
		public:
			static void Initialize();
			inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
			inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		private:
			static std::shared_ptr<spdlog::logger> s_CoreLogger;
			static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
} // EEngine
