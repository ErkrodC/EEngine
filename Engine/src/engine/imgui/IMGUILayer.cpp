#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Platform/Windows/WindowsWindow.hpp>
#include "IMGUILayer.hpp"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Application.hpp"

// ER TEMP: remove hackish includes
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
		//EE_CORE_TRACE("IMGUI Layer: {0}", event);

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
		KeyCode keyCode = event.GetKeyCode();
		ImGuiKey imguiKey = EngineToIMGUIKeyCode(keyCode);
		io.AddKeyEvent(imguiKey, true);

		switch (keyCode) {
			case KeyCode::LeftControl:
			case KeyCode::RightControl: {
				io.AddKeyEvent(ImGuiMod_Ctrl, true);
				break;
			}
			case KeyCode::LeftShift:
			case KeyCode::RightShift: {
				io.AddKeyEvent(ImGuiMod_Shift, true);
				break;
			}
			case KeyCode::LeftAlt:
			case KeyCode::RightAlt: {
				io.AddKeyEvent(ImGuiMod_Alt, true);
				break;
			}
			case KeyCode::LeftSuper:
			case KeyCode::RightSuper: {
				io.AddKeyEvent(ImGuiMod_Super, true);
				break;
			}
		}

		return false;
	}

	bool IMGUILayer::OnKeyReleased(KeyReleasedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		KeyCode keyCode = event.GetKeyCode();
		ImGuiKey imguiKey = EngineToIMGUIKeyCode(keyCode);
		io.AddKeyEvent(imguiKey, false);

		switch (keyCode) {
			case KeyCode::LeftControl:
			case KeyCode::RightControl: {
				io.AddKeyEvent(ImGuiMod_Ctrl, false);
				break;
			}
			case KeyCode::LeftShift:
			case KeyCode::RightShift: {
				io.AddKeyEvent(ImGuiMod_Shift, false);
				break;
			}
			case KeyCode::LeftAlt:
			case KeyCode::RightAlt: {
				io.AddKeyEvent(ImGuiMod_Alt, false);
				break;
			}
			case KeyCode::LeftSuper:
			case KeyCode::RightSuper: {
				io.AddKeyEvent(ImGuiMod_Super, false);
				break;
			}
		}

		return false;
	}

	bool IMGUILayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();

		int button = (int)event.GetMouseButton();
		if (button >= 0 && button < ImGuiMouseButton_COUNT) {
			io.AddMouseButtonEvent(button, true);
		}

		return false;
	}

	bool IMGUILayer::OnMouseButtonReleased(MouseButtonReleasedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();

		int button = (int)event.GetMouseButton();
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

	ImGuiKey IMGUILayer::EngineToIMGUIKeyCode(KeyCode key) {
		switch (key) {
			case KeyCode::Tab: return ImGuiKey_Tab;
			case KeyCode::Left: return ImGuiKey_LeftArrow;
			case KeyCode::Right: return ImGuiKey_RightArrow;
			case KeyCode::Up: return ImGuiKey_UpArrow;
			case KeyCode::Down: return ImGuiKey_DownArrow;
			case KeyCode::PageUp: return ImGuiKey_PageUp;
			case KeyCode::PageDown: return ImGuiKey_PageDown;
			case KeyCode::Home: return ImGuiKey_Home;
			case KeyCode::End: return ImGuiKey_End;
			case KeyCode::Insert: return ImGuiKey_Insert;
			case KeyCode::Delete: return ImGuiKey_Delete;
			case KeyCode::Backspace: return ImGuiKey_Backspace;
			case KeyCode::Space: return ImGuiKey_Space;
			case KeyCode::Enter: return ImGuiKey_Enter;
			case KeyCode::Escape: return ImGuiKey_Escape;
			case KeyCode::Apostrophe: return ImGuiKey_Apostrophe;
			case KeyCode::Comma: return ImGuiKey_Comma;
			case KeyCode::Minus: return ImGuiKey_Minus;
			case KeyCode::Period: return ImGuiKey_Period;
			case KeyCode::Slash: return ImGuiKey_Slash;
			case KeyCode::Semicolon: return ImGuiKey_Semicolon;
			case KeyCode::Equal: return ImGuiKey_Equal;
			case KeyCode::LeftBracket: return ImGuiKey_LeftBracket;
			case KeyCode::Backslash: return ImGuiKey_Backslash;
			case KeyCode::RightBracket: return ImGuiKey_RightBracket;
			case KeyCode::GraveAccent: return ImGuiKey_GraveAccent;
			case KeyCode::CapsLock: return ImGuiKey_CapsLock;
			case KeyCode::ScrollLock: return ImGuiKey_ScrollLock;
			case KeyCode::NumLock: return ImGuiKey_NumLock;
			case KeyCode::PrintScreen: return ImGuiKey_PrintScreen;
			case KeyCode::Pause: return ImGuiKey_Pause;
			case KeyCode::Numpad0: return ImGuiKey_Keypad0;
			case KeyCode::Numpad1: return ImGuiKey_Keypad1;
			case KeyCode::Numpad2: return ImGuiKey_Keypad2;
			case KeyCode::Numpad3: return ImGuiKey_Keypad3;
			case KeyCode::Numpad4: return ImGuiKey_Keypad4;
			case KeyCode::Numpad5: return ImGuiKey_Keypad5;
			case KeyCode::Numpad6: return ImGuiKey_Keypad6;
			case KeyCode::Numpad7: return ImGuiKey_Keypad7;
			case KeyCode::Numpad8: return ImGuiKey_Keypad8;
			case KeyCode::Numpad9: return ImGuiKey_Keypad9;
			case KeyCode::NumpadDecimal: return ImGuiKey_KeypadDecimal;
			case KeyCode::NumpadDivide: return ImGuiKey_KeypadDivide;
			case KeyCode::NumpadMultiply: return ImGuiKey_KeypadMultiply;
			case KeyCode::NumpadSubtract: return ImGuiKey_KeypadSubtract;
			case KeyCode::NumpadAdd: return ImGuiKey_KeypadAdd;
			case KeyCode::NumpadEnter: return ImGuiKey_KeypadEnter;
			case KeyCode::NumpadEqual: return ImGuiKey_KeypadEqual;
			case KeyCode::LeftShift: return ImGuiKey_LeftShift;
			case KeyCode::LeftControl: return ImGuiKey_LeftCtrl;
			case KeyCode::LeftAlt: return ImGuiKey_LeftAlt;
			case KeyCode::LeftSuper: return ImGuiKey_LeftSuper;
			case KeyCode::RightShift: return ImGuiKey_RightShift;
			case KeyCode::RightControl: return ImGuiKey_RightCtrl;
			case KeyCode::RightAlt: return ImGuiKey_RightAlt;
			case KeyCode::RightSuper: return ImGuiKey_RightSuper;
			case KeyCode::Menu: return ImGuiKey_Menu;
			case KeyCode::_0: return ImGuiKey_0;
			case KeyCode::_1: return ImGuiKey_1;
			case KeyCode::_2: return ImGuiKey_2;
			case KeyCode::_3: return ImGuiKey_3;
			case KeyCode::_4: return ImGuiKey_4;
			case KeyCode::_5: return ImGuiKey_5;
			case KeyCode::_6: return ImGuiKey_6;
			case KeyCode::_7: return ImGuiKey_7;
			case KeyCode::_8: return ImGuiKey_8;
			case KeyCode::_9: return ImGuiKey_9;
			case KeyCode::A: return ImGuiKey_A;
			case KeyCode::B: return ImGuiKey_B;
			case KeyCode::C: return ImGuiKey_C;
			case KeyCode::D: return ImGuiKey_D;
			case KeyCode::E: return ImGuiKey_E;
			case KeyCode::F: return ImGuiKey_F;
			case KeyCode::G: return ImGuiKey_G;
			case KeyCode::H: return ImGuiKey_H;
			case KeyCode::I: return ImGuiKey_I;
			case KeyCode::J: return ImGuiKey_J;
			case KeyCode::K: return ImGuiKey_K;
			case KeyCode::L: return ImGuiKey_L;
			case KeyCode::M: return ImGuiKey_M;
			case KeyCode::N: return ImGuiKey_N;
			case KeyCode::O: return ImGuiKey_O;
			case KeyCode::P: return ImGuiKey_P;
			case KeyCode::Q: return ImGuiKey_Q;
			case KeyCode::R: return ImGuiKey_R;
			case KeyCode::S: return ImGuiKey_S;
			case KeyCode::T: return ImGuiKey_T;
			case KeyCode::U: return ImGuiKey_U;
			case KeyCode::V: return ImGuiKey_V;
			case KeyCode::W: return ImGuiKey_W;
			case KeyCode::X: return ImGuiKey_X;
			case KeyCode::Y: return ImGuiKey_Y;
			case KeyCode::Z: return ImGuiKey_Z;
			case KeyCode::F1: return ImGuiKey_F1;
			case KeyCode::F2: return ImGuiKey_F2;
			case KeyCode::F3: return ImGuiKey_F3;
			case KeyCode::F4: return ImGuiKey_F4;
			case KeyCode::F5: return ImGuiKey_F5;
			case KeyCode::F6: return ImGuiKey_F6;
			case KeyCode::F7: return ImGuiKey_F7;
			case KeyCode::F8: return ImGuiKey_F8;
			case KeyCode::F9: return ImGuiKey_F9;
			case KeyCode::F10: return ImGuiKey_F10;
			case KeyCode::F11: return ImGuiKey_F11;
			case KeyCode::F12: return ImGuiKey_F12;
			default: return ImGuiKey_None;
		}
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