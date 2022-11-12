#include <GLFW/glfw3.h>
#include <Events/KeyEvent.hpp>
#include "IMGUILayer.hpp"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
//#include "backends/imgui_impl_glfw.h"
#include "Application.hpp"

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
#define IMGUI_MAP_GLFW_KEY(imguiKey, glfwKey) io.KeyMap[ImGuiKey_##imguiKey] = GLFW_KEY_##glfwKey
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
		IMGUI_MAP_GLFW_KEY(Z, Z);

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
	}

	bool IMGUILayer::OnKeyPressed(KeyPressedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;
		return true;
	}

	bool IMGUILayer::OnKeyReleased(KeyReleasedEvent& event) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = false;
		return true;
	}
} // EEngine