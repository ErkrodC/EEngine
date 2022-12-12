export module EEngine.Application:Input;
import :IInput;
import EEngine.Core;
import EEngine.std.core;

namespace EEngine::Input {
	static IInput* s_Instance;

	export inline void SetInputInstance(IInput* instance) { s_Instance = instance; }
	export inline bool IsKeyPressed(KeyCode keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
	export inline bool IsMouseButtonPressed(MouseButtonCode mouseButtonCode) { return s_Instance->IsMouseButtonPressedImpl(mouseButtonCode); }
	export inline std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
	export inline float GetMouseX() { return s_Instance->GetMouseXImpl(); }
	export inline float GetMouseY() { return s_Instance->GetMouseYImpl(); }

#if WIN32
	export inline int EngineToGLFWKeyCode(KeyCode engineKeyCode) { return (int)engineKeyCode; }
	export inline int EngineToGLFWMouseButtonCode(MouseButtonCode engineMouseButtonCode) { return (int)engineMouseButtonCode; }
	export inline KeyCode GLFWToEngineKeyCode(int glfwKeyCode) { return (KeyCode)glfwKeyCode; }
	export inline MouseButtonCode GLFWToEngineMouseButtonCode(int glfwMouseButtonCode) { return (MouseButtonCode)glfwMouseButtonCode; }
// #else // ER NOTE add else if using some other platform-specific keycodes
#endif
};