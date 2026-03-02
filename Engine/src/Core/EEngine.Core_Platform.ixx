export module EEngine.Core:Platform;
import EEngine.Standard;
import :Input;

export namespace EEngine {
	enum class Platform { MacOS, Linux, Windows };

#if EE_PLATFORM_MACOS
	constexpr Platform g_Platform = Platform::MacOS;
#elif EE_PLATFORM_LINUX
	constexpr Platform g_Platform = Platform::Linux;
#elif EE_PLATFORM_WINDOWS
	constexpr Platform g_Platform = Platform::Windows;
#else
	#error Platform not supported!
#endif

	namespace Windows {
		inline KeyCode GLFWToEngineKeyCode(unsigned int glfwKeyCode) { return static_cast<KeyCode>(glfwKeyCode); }
		inline MouseButtonCode GLFWToEngineMouseButtonCode(int glfwMouseButtonCode) { return static_cast<MouseButtonCode>(glfwMouseButtonCode); }
		inline int32_t EngineToGLFWKeyCode(KeyCode engineKeyCode) { return static_cast<int32_t>(engineKeyCode); }
		inline int32_t EngineToGLFWMouseButtonCode(MouseButtonCode engineMouseButtonCode) { return static_cast<int32_t>(engineMouseButtonCode); }
	}
}