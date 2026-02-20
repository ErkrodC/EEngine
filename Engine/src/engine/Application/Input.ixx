export module EEngine.Application:Input;
import :IInput;
import :WindowsInput;
import EEngine.Core;
import EEngine.Standard;

namespace EEngine::Input {
	inline std::unique_ptr<IInput>& GetInstance() {
		static std::unique_ptr<IInput> instance = nullptr;

		if (!instance) {
#ifdef EE_PLATFORM_WINDOWS
			instance = std::make_unique<WindowsInput>();
#endif
		}

		return instance;
	}

	export inline void SetInputInstance(std::unique_ptr<IInput> instance) { GetInstance() = std::move(instance); }
	export inline void SetWindow(void* window) {
#ifdef EE_PLATFORM_WINDOWS
		if (auto* windowsInput = dynamic_cast<WindowsInput*>(GetInstance().get())) {
			windowsInput->SetWindow(window);
		}
#endif
	}
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