module;
#include "GLFW/glfw3.h"

export module EEngine.Application:WindowsInput;
import :IInput;
import :IWindow;
import EEngine.Core;

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

			return state == GLFW_PRESS
				|| state == GLFW_REPEAT;
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
} // EEngine
