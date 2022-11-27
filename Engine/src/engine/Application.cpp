#include "Application.hpp"
#include <glad/glad.h>

#include "Input.hpp"

namespace EEngine {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		EE_CORE_ASSERT(!s_Instance, "Multiple applications created.");

		Application::s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_IMGUILayer = new IMGUILayer();
		PushOverlay(m_IMGUILayer);
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(event);
			if (event.Handled) { break; }
		}
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			// ER TODO will eventually be on a "render" thread, isolated from update/application thread
			m_IMGUILayer->Begin();
			for (Layer* layer: m_LayerStack) {
				layer->OnIMGUIRender();
			}
			m_IMGUILayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(EEngine::WindowCloseEvent&) {
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(EEngine::Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
} // Engine