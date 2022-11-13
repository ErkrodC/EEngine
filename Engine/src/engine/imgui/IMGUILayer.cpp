#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Platform/Windows/WindowsWindow.hpp>
#include "IMGUILayer.hpp"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Application.hpp"

// ER TEMP: remove hackish includes
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace EEngine {
	IMGUILayer::IMGUILayer()
		: Layer("IMGUILayer") {

	}

	IMGUILayer::~IMGUILayer() {

	}

	void IMGUILayer::OnAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// ER TEMP: should use EEngine KeyCodes eventually
/*#define IMGUI_MAP_GLFW_KEY(imguiKey, glfwKey) io.KeyMap[ImGuiKey_##imguiKey] = GLFW_KEY_##glfwKey
		IMGUI_MAP_GLFW_KEY(Tab, TAB);
		IMGUI_MAP_GLFW_KEY(LeftArrow, LEFT);
		IMGUI_MAP_GLFW_KEY(RightArrow, RIGHT);
		IMGUI_MAP_GLFW_KEY(UpArrow, UP);
		IMGUI_MAP_GLFW_KEY(DownArrow, DOWN);
		IMGUI_MAP_GLFW_KEY(PageUp, PAGE_UP);
		IMGUI_MAP_GLFW_KEY(PageDown, PAGE_DOWN);
		IMGUI_MAP_GLFW_KEY(Home, HOME);
		IMGUI_MAP_GLFW_KEY(End, END);
		IMGUI_MAP_GLFW_KEY(Insert, INSERT);
		IMGUI_MAP_GLFW_KEY(Delete, DELETE);
		IMGUI_MAP_GLFW_KEY(Backspace, BACKSPACE);
		IMGUI_MAP_GLFW_KEY(Space, SPACE);
		IMGUI_MAP_GLFW_KEY(Enter, ENTER);
		IMGUI_MAP_GLFW_KEY(Escape, ESCAPE);
		IMGUI_MAP_GLFW_KEY(A, A);
		IMGUI_MAP_GLFW_KEY(C, C);
		IMGUI_MAP_GLFW_KEY(V, V);
		IMGUI_MAP_GLFW_KEY(X, X);
		IMGUI_MAP_GLFW_KEY(Y, Y);
		IMGUI_MAP_GLFW_KEY(Z, Z);*/

		auto glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow , true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void IMGUILayer::OnDetach() {

	}

	void IMGUILayer::OnUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		Window& appWindow = Application::Get().GetWindow();
		io.DisplaySize = ImVec2(
			(float)appWindow.GetWidth(),
			(float)appWindow.GetHeight()
		);

		auto time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f
			? (time -  m_Time)
			: (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void IMGUILayer::OnEvent(Event& event) {
		EE_CORE_TRACE("IMGUI Layer: {0}", event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(IMGUILayer::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(IMGUILayer::OnKeyReleased));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(IMGUILayer::OnKeyTyped));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(IMGUILayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(IMGUILayer::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(IMGUILayer::OnMouseScrolled));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(IMGUILayer::OnMouseMoved));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(IMGUILayer::OnWindowResized));
	}

	bool IMGUILayer::OnKeyPressed(KeyPressedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		int keyCode = event.GetKeyCode();
		ImGuiKey imguiKey = GLFWKeyToIMGUIKey(keyCode);
		io.AddKeyEvent(imguiKey, true);

		int mod = GLFWKeyToModifier(keyCode);
		switch (mod) {
			case GLFW_MOD_CONTROL: {
				io.AddKeyEvent(ImGuiMod_Ctrl, true);
				break;
			}
			case GLFW_MOD_SHIFT: {
				io.AddKeyEvent(ImGuiMod_Shift, true);
				break;
			}
			case GLFW_MOD_ALT: {
				io.AddKeyEvent(ImGuiMod_Alt, true);
				break;
			}
			case GLFW_MOD_SUPER: {
				io.AddKeyEvent(ImGuiMod_Super, true);
				break;
			}
		}

		return false;
	}

	bool IMGUILayer::OnKeyReleased(KeyReleasedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		int keyCode = event.GetKeyCode();
		ImGuiKey imguiKey = GLFWKeyToIMGUIKey(keyCode);
		io.AddKeyEvent(imguiKey, false);

		int mod = GLFWKeyToModifier(keyCode);
		switch (mod) {
			case GLFW_MOD_CONTROL: {
				io.AddKeyEvent(ImGuiMod_Ctrl, false);
				break;
			}
			case GLFW_MOD_SHIFT: {
				io.AddKeyEvent(ImGuiMod_Shift, false);
				break;
			}
			case GLFW_MOD_ALT: {
				io.AddKeyEvent(ImGuiMod_Alt, false);
				break;
			}
			case GLFW_MOD_SUPER: {
				io.AddKeyEvent(ImGuiMod_Super, false);
				break;
			}
		}

		return false;
	}

	bool IMGUILayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();

		int button = event.GetMouseButton();
		if (button >= 0 && button < ImGuiMouseButton_COUNT) {
			io.AddMouseButtonEvent(button, true);
		}

		return false;
	}

	bool IMGUILayer::OnMouseButtonReleased(MouseButtonReleasedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();

		int button = event.GetMouseButton();
		if (button >= 0 && button < ImGuiMouseButton_COUNT) {
			io.AddMouseButtonEvent(button, false);
		}

		return false;
	}

	bool IMGUILayer::OnMouseScrolled(MouseScrolledEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent((float) event.GetXOffset(), (float) event.GetYOffset());

		return false;
	}

	ImGuiKey IMGUILayer::GLFWKeyToIMGUIKey(int key) {
		switch (key) {
			case GLFW_KEY_TAB: return ImGuiKey_Tab;
			case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
			case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
			case GLFW_KEY_UP: return ImGuiKey_UpArrow;
			case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
			case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
			case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
			case GLFW_KEY_HOME: return ImGuiKey_Home;
			case GLFW_KEY_END: return ImGuiKey_End;
			case GLFW_KEY_INSERT: return ImGuiKey_Insert;
			case GLFW_KEY_DELETE: return ImGuiKey_Delete;
			case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
			case GLFW_KEY_SPACE: return ImGuiKey_Space;
			case GLFW_KEY_ENTER: return ImGuiKey_Enter;
			case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
			case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
			case GLFW_KEY_COMMA: return ImGuiKey_Comma;
			case GLFW_KEY_MINUS: return ImGuiKey_Minus;
			case GLFW_KEY_PERIOD: return ImGuiKey_Period;
			case GLFW_KEY_SLASH: return ImGuiKey_Slash;
			case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
			case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
			case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
			case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
			case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
			case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
			case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
			case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
			case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
			case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
			case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
			case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
			case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
			case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
			case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
			case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
			case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
			case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
			case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
			case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
			case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
			case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
			case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
			case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
			case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
			case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
			case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
			case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
			case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
			case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
			case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
			case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
			case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
			case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
			case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
			case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
			case GLFW_KEY_MENU: return ImGuiKey_Menu;
			case GLFW_KEY_0: return ImGuiKey_0;
			case GLFW_KEY_1: return ImGuiKey_1;
			case GLFW_KEY_2: return ImGuiKey_2;
			case GLFW_KEY_3: return ImGuiKey_3;
			case GLFW_KEY_4: return ImGuiKey_4;
			case GLFW_KEY_5: return ImGuiKey_5;
			case GLFW_KEY_6: return ImGuiKey_6;
			case GLFW_KEY_7: return ImGuiKey_7;
			case GLFW_KEY_8: return ImGuiKey_8;
			case GLFW_KEY_9: return ImGuiKey_9;
			case GLFW_KEY_A: return ImGuiKey_A;
			case GLFW_KEY_B: return ImGuiKey_B;
			case GLFW_KEY_C: return ImGuiKey_C;
			case GLFW_KEY_D: return ImGuiKey_D;
			case GLFW_KEY_E: return ImGuiKey_E;
			case GLFW_KEY_F: return ImGuiKey_F;
			case GLFW_KEY_G: return ImGuiKey_G;
			case GLFW_KEY_H: return ImGuiKey_H;
			case GLFW_KEY_I: return ImGuiKey_I;
			case GLFW_KEY_J: return ImGuiKey_J;
			case GLFW_KEY_K: return ImGuiKey_K;
			case GLFW_KEY_L: return ImGuiKey_L;
			case GLFW_KEY_M: return ImGuiKey_M;
			case GLFW_KEY_N: return ImGuiKey_N;
			case GLFW_KEY_O: return ImGuiKey_O;
			case GLFW_KEY_P: return ImGuiKey_P;
			case GLFW_KEY_Q: return ImGuiKey_Q;
			case GLFW_KEY_R: return ImGuiKey_R;
			case GLFW_KEY_S: return ImGuiKey_S;
			case GLFW_KEY_T: return ImGuiKey_T;
			case GLFW_KEY_U: return ImGuiKey_U;
			case GLFW_KEY_V: return ImGuiKey_V;
			case GLFW_KEY_W: return ImGuiKey_W;
			case GLFW_KEY_X: return ImGuiKey_X;
			case GLFW_KEY_Y: return ImGuiKey_Y;
			case GLFW_KEY_Z: return ImGuiKey_Z;
			case GLFW_KEY_F1: return ImGuiKey_F1;
			case GLFW_KEY_F2: return ImGuiKey_F2;
			case GLFW_KEY_F3: return ImGuiKey_F3;
			case GLFW_KEY_F4: return ImGuiKey_F4;
			case GLFW_KEY_F5: return ImGuiKey_F5;
			case GLFW_KEY_F6: return ImGuiKey_F6;
			case GLFW_KEY_F7: return ImGuiKey_F7;
			case GLFW_KEY_F8: return ImGuiKey_F8;
			case GLFW_KEY_F9: return ImGuiKey_F9;
			case GLFW_KEY_F10: return ImGuiKey_F10;
			case GLFW_KEY_F11: return ImGuiKey_F11;
			case GLFW_KEY_F12: return ImGuiKey_F12;
			default: return ImGuiKey_None;
		}
	}

	int IMGUILayer::GLFWKeyToModifier(int key) {
		if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
			return GLFW_MOD_CONTROL;
		}
		if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
			return GLFW_MOD_SHIFT;
		}
		if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
			return GLFW_MOD_ALT;
		}
		if (key == GLFW_KEY_LEFT_SUPER || key == GLFW_KEY_RIGHT_SUPER) {
			return GLFW_MOD_SUPER;
		}
		return 0;
	}

	bool IMGUILayer::OnMouseMoved(MouseMovedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(event.GetX(), event.GetY());

		return false;
	}

	bool IMGUILayer::OnWindowResized(WindowResizeEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)event.GetWidth(), (float)event.GetHeight());
		glViewport(0, 0, event.GetWidth(), event.GetHeight());

		return false;
	}

	bool IMGUILayer::OnKeyTyped(KeyTypedEvent& event) {
		// ER NOTE, seems to be legacy to ImGui to expect this.
		// uncommenting causes keys to be typed twice.

		/*ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(event.GetKeyCode());*/

		return false;
	}
} // EEngine