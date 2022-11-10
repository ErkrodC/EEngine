#include "Application.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Log.hpp"

namespace EEngine {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		EE_CORE_INFO("Welcome to EEngine!");
		WindowResizeEvent e(1280, 720);
		EE_TRACE(e);

		while (true) {}
	}
} // Engine