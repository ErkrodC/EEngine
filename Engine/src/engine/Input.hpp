#pragma once

#include "Core.hpp"
#include "KeyCode.hpp"
#include "MouseButtonCode.hpp"

namespace EEngine {
	class Input {
	public:
		inline static bool IsKeyPressed(KeyCode keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
		inline static bool IsMouseButtonPressed(MouseButtonCode mouseButtonCode) { return s_Instance->IsMouseButtonPressedImpl(mouseButtonCode); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseButtonCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_Instance;

	public:
#if WIN32
		static inline int EngineToGLFWKeyCode(KeyCode engineKeyCode) {
			return (int)engineKeyCode;
		}

		static int EngineToGLFWMouseButtonCode(MouseButtonCode engineMouseButtonCode) {
			return (int)engineMouseButtonCode;
		}

		static inline KeyCode GLFWToEngineKeyCode(int glfwKeyCode) {
			return (KeyCode)glfwKeyCode;
		}

		static inline MouseButtonCode GLFWToEngineMouseButtonCode(int glfwMouseButtonCode) {
			return (MouseButtonCode)glfwMouseButtonCode;
		}
// #else // ER NOTE add else if using some other platform-specific keycodes
#endif
	};
}