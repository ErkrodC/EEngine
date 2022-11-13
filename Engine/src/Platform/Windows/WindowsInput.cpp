#include "WindowsInput.hpp"

#include "Application.hpp"
#include "GLFW/glfw3.h"

namespace EEngine {
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keyCode) {
		auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(glfwWindow, keyCode);

		return state == GLFW_PRESS
			|| state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button) {
		auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(glfwWindow, button);

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