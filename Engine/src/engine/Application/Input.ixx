export module EEngine.Application:Input;
import :IInput;
import EEngine.Core;
import EEngine.Standard;

namespace EEngine::Input {
	inline IInput*& GetInstance() {
		static IInput* instance = nullptr;
		return instance;
	}

	export inline void SetInputInstance(IInput* instance) { GetInstance() = instance; }
	export inline bool IsKeyPressed(KeyCode keyCode) { return GetInstance()->IsKeyPressedImpl(keyCode); }
	export inline bool IsMouseButtonPressed(MouseButtonCode mouseButtonCode) { return GetInstance()->IsMouseButtonPressedImpl(mouseButtonCode); }
	export inline std::pair<float, float> GetMousePosition() { return GetInstance()->GetMousePositionImpl(); }
	export inline float GetMouseX() { return GetInstance()->GetMouseXImpl(); }
	export inline float GetMouseY() { return GetInstance()->GetMouseYImpl(); }

#if WIN32
	export inline int EngineToGLFWKeyCode(KeyCode engineKeyCode) { return (int)engineKeyCode; }
	export inline int EngineToGLFWMouseButtonCode(MouseButtonCode engineMouseButtonCode) { return (int)engineMouseButtonCode; }
	export inline KeyCode GLFWToEngineKeyCode(int glfwKeyCode) { return (KeyCode)glfwKeyCode; }
	export inline MouseButtonCode GLFWToEngineMouseButtonCode(int glfwMouseButtonCode) { return (MouseButtonCode)glfwMouseButtonCode; }
// #else // ER NOTE add else if using some other platform-specific keycodes
#endif
};