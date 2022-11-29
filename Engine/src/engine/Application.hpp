#pragma once

#include <Renderer/Shader.hpp>
#include "Core.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Window.hpp"
#include "imgui/IMGUILayer.hpp"

namespace EEngine {
	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() const { return *m_Window; }
	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		IMGUILayer* m_IMGUILayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		bool OnWindowClose(WindowCloseEvent& event);
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
