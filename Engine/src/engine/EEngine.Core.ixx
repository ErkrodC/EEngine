module;
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

export module EEngine.Core;
import EEngine.Standard;

export namespace EEngine {
	// ============================================================================
	// Smart Pointer Aliases
	// ============================================================================

	// Ref<T> = Reference with exclusive ownership (wraps std::unique_ptr)
	// Use for single-owner resources (most common case)
	template<typename T>
	using Ref = std::unique_ptr<T>;

	// Shared<T> = Reference with shared ownership (wraps std::shared_ptr)
	// Use when multiple owners need to keep resource alive
	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> MakeRef(Args &&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	constexpr Shared<T> MakeShared(Args &&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// ============================================================================
	// Timestep
	// ============================================================================

	class Timestep {
	public:
		explicit Timestep(float time = 0.0f)
			: m_Time(time) {}

		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};

	// ============================================================================
	// Input Codes
	// ============================================================================

	enum class KeyCode {
		/* Printable Keys */
		Space = 32,
		Apostrophe = 39,
		Comma = 44,
		Minus = 45,
		Period = 46,
		Slash = 47,
		_0 = 48,
		_1 = 49,
		_2 = 50,
		_3 = 51,
		_4 = 52,
		_5 = 53,
		_6 = 54,
		_7 = 55,
		_8 = 56,
		_9 = 57,
		Semicolon = 59,
		Equal = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LeftBracket = 91,
		Backslash = 92,
		RightBracket = 93,
		GraveAccent = 96,
		World1 = 161,
		World2 = 162,

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		Numpad0 = 320,
		Numpad1 = 321,
		Numpad2 = 322,
		Numpad3 = 323,
		Numpad4 = 324,
		Numpad5 = 325,
		Numpad6 = 326,
		Numpad7 = 327,
		Numpad8 = 328,
		Numpad9 = 329,
		NumpadDecimal = 330,
		NumpadDivide = 331,
		NumpadMultiply = 332,
		NumpadSubtract = 333,
		NumpadAdd = 334,
		NumpadEnter = 335,
		NumpadEqual = 336,
		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348,
	};

	enum class MouseButtonCode {
		Mouse1 = 0,
		Mouse2 = 1,
		Mouse3 = 2,
		Mouse4 = 3,
		Mouse5 = 4,
		Mouse6 = 5,
		Mouse7 = 6,
		Mouse8 = 7,
	};
}

// ============================================================================
// Logging
// ============================================================================
namespace EEngine::Log {
	static Shared<spdlog::logger> s_CoreLogger = nullptr;
	static Shared<spdlog::logger> s_ClientLogger = nullptr;

	export void Initialize() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("EENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	export inline Shared<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	export inline Shared<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

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
#else
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
#endif
}
