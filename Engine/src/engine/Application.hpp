#pragma once

#include "Core.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Window.hpp"

namespace EEngine {
	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
