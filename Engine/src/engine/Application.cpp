#include "Application.hpp"

namespace EEngine {
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		EE_CORE_TRACE(event);
	}

	void Application::Run() {
		while (m_Running) {
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(EEngine::WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}
} // Engine