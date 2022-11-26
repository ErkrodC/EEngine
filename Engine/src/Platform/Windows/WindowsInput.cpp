#include "WindowsInput.hpp"

#include "Application.hpp"
#include "KeyCode.hpp"
#include "GLFW/glfw3.h"

namespace EEngine {
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(KeyCode keyCode) {
		auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(glfwWindow, Input::EngineToGLFWKeyCode(keyCode));

		return state == GLFW_PRESS
			|| state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(MouseButtonCode mouseButtonCode) {
		auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(glfwWindow, Input::EngineToGLFWMouseButtonCode(mouseButtonCode));

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl() {
		auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(glfwWindow, &x, &y);

		return { (float)x, (float)y };
	}

	float WindowsInput::GetMouseXImpl() {
		auto[x, y] = GetMousePositionImpl();

		return x;
	}

	float WindowsInput::GetMouseYImpl() {
		auto[x, y] = GetMousePositionImpl();

		return y;
	}
} // EEngine