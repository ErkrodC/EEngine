#include "Application.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/RendererAPI.hpp>
#include <memory>

#include "Input.hpp"

namespace EEngine {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		EE_CORE_INFO("Selected Renderer API: {0}", Renderer::GetRendererAPIString(Renderer::GetSelectedAPI()));
		EE_CORE_ASSERT(!s_Instance, "Multiple applications created.");

		Application::s_Instance = this;

		m_Window = std::unique_ptr<IWindow>(IWindow::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_IMGUILayer = new IMGUILayer();
		PushOverlay(m_IMGUILayer);
	}

	Application::~Application() = default;

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(event);
			if (event.Handled) { break; }
		}
	}

	void Application::Run() {
		while (m_Running) {
			{
				RendererAPI::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

				Renderer::BeginScene(); {
					OnSceneUpdate();
				} Renderer::EndScene();

				// ER TODO usually executed on a separate thread
				//Renderer::Flush();
			}

			{
				for (Layer* layer: m_LayerStack) {
					layer->OnUpdate();
				}
			}

			// ER TODO will eventually be on a "render" thread, isolated from update/application thread
			{
				m_IMGUILayer->Begin();
				for (Layer* layer: m_LayerStack) {
					layer->OnIMGUIRender();
				}
				m_IMGUILayer->End();
			}

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