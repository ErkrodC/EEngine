module;
#include <utility>
#include "GLFW/glfw3.h"

export module EEngine.Application:WindowsInput;
import :Application;
import :IInput;
import :Input;
import EEngine.Core;

export namespace EEngine {
	class WindowsInput : public IInput {
	protected:
		bool IsKeyPressedImpl(KeyCode keyCode) override  {
			auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
			auto state = glfwGetKey(glfwWindow, Input::EngineToGLFWKeyCode(keyCode));

			return state == GLFW_PRESS
				|| state == GLFW_REPEAT;
		}

		bool IsMouseButtonPressedImpl(MouseButtonCode mouseButtonCode) override {
			auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
			auto state = glfwGetMouseButton(glfwWindow, Input::EngineToGLFWMouseButtonCode(mouseButtonCode));

			return state == GLFW_PRESS;
		}

		std::pair<float, float> GetMousePositionImpl() override {
			auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
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
	};
} // EEngine
