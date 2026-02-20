module;
#include <GLFW/glfw3.h>

export module EEngine.Application:Input;
import EEngine.Core;
import EEngine.Standard;
import :Window;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Input Interface
	// ============================================================================

	class IInput {
	public:
		virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseButtonCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	};
}

// ============================================================================
// Platform-specific Implementations (Windows)
// ============================================================================

namespace EEngine {
#if WIN32
	inline int EngineToGLFWKeyCode(KeyCode engineKeyCode) { return (int)engineKeyCode; }
	inline int EngineToGLFWMouseButtonCode(MouseButtonCode engineMouseButtonCode) { return (int)engineMouseButtonCode; }
#endif
}

export namespace EEngine {
	class WindowsInput : public IInput {
	public:
		void SetWindow(void* window) {
			m_Window = static_cast<IWindow*>(window);
		}

	protected:
		bool IsKeyPressedImpl(KeyCode keyCode) override  {
			if (!m_Window) { return false; }
			auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
			auto state = glfwGetKey(glfwWindow, EngineToGLFWKeyCode(keyCode));

			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		bool IsMouseButtonPressedImpl(MouseButtonCode mouseButtonCode) override {
			if (!m_Window) { return false; }
			auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
			auto state = glfwGetMouseButton(glfwWindow, EngineToGLFWMouseButtonCode(mouseButtonCode));

			return state == GLFW_PRESS;
		}

		std::pair<float, float> GetMousePositionImpl() override {
			if (!m_Window) return { 0.0f, 0.0f };
			auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
			double x, y;
			glfwGetCursorPos(glfwWindow, &x, &y);

			return { (float)x, (float)y };
		}

		float GetMouseXImpl() override {
			auto[x, y] = GetMousePositionImpl();
			return x;
		}

		float GetMouseYImpl() override {
			auto[x, y] = GetMousePositionImpl();
			return y;
		}

	private:
		IWindow* m_Window = nullptr;
	};
}

// ============================================================================
// Input Facade
// ============================================================================

namespace EEngine::Input {
	inline Ref<IInput>& GetInstance() {
		static Ref<IInput> instance = nullptr;

		if (!instance) {
#ifdef EE_PLATFORM_WINDOWS
			instance = MakeRef<WindowsInput>();
#endif
		}

		return instance;
	}

	export inline void SetInputInstance(Ref<IInput> instance) { GetInstance() = std::move(instance); }
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
}
