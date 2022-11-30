#pragma once

#include <Renderer/Shader.hpp>
#include <Renderer/Buffer.hpp>
#include <Renderer/VertexArray.hpp>
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
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		bool OnWindowClose(WindowCloseEvent& event);
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
