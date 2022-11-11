#pragma once

#include "Core.hpp"
#include "Window.hpp"

namespace EEngine {
	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
