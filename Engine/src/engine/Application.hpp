#pragma once

#include <Events/ApplicationEvent.hpp>
#include "Core.hpp"
#include "Window.hpp"

namespace EEngine {
	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& event);

		bool OnWindowClose(WindowCloseEvent& event);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
