#pragma once

#include <imgui.h>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include "Layer.hpp"

namespace EEngine {
	class IMGUILayer : public Layer {
	public:
		IMGUILayer();
		~IMGUILayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event &event) override;

	private:
		float m_Time = 0.0f;

		static ImGuiKey GLFWKeyToIMGUIKey(int key);
		int GLFWKeyToModifier(int key);

		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
		bool OnKeyTyped(KeyTypedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);
	};
} // EEngine
