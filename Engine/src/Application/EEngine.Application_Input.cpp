module;
#include <GLFW/glfw3.h>

module EEngine.Application;
import :Input;

namespace EEngine {
	bool WindowsInput::IsKeyPressed(KeyCode keyCode) const {
		if (!m_Window) { return false; }
		auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
		auto state = glfwGetKey(glfwWindow, Windows::EngineToGLFWKeyCode(keyCode));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressed(MouseButtonCode mouseButtonCode) const {
		if (!m_Window) { return false; }
		auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
		auto state = glfwGetMouseButton(glfwWindow, Windows::EngineToGLFWMouseButtonCode(mouseButtonCode));

		return state == GLFW_PRESS;
	}

	std::pair<float_t, float_t> WindowsInput::GetMousePosition() const {
		if (!m_Window) return { 0.0f, 0.0f };
		auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
		double x, y;
		glfwGetCursorPos(glfwWindow, &x, &y);

		return { static_cast<float_t>(x), static_cast<float_t>(y) };
	}
}