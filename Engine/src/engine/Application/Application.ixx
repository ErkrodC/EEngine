module;
#include <memory>
#include <GLFW/glfw3.h>
#include "Core/Core.hpp"

export module EEngine.Application:Application;
import :IInput;
import :IMGUILayer;
import :IWindow;
import :Layer;
import :LayerStack;
import :TBD;
import EEngine.Core;
import EEngine.Event;
import EEngine.Rendering;

export namespace EEngine {
	class Application {
	public:
		Application() {
			EE_CORE_INFO("Selected Renderer API: {0}", Renderer::GetRendererAPIString(Renderer::GetSelectedAPI()));
			EE_CORE_ASSERT(!s_Instance, "Multiple applications created.");

			Application::s_Instance = this;

			m_Window.reset(TBD::CreateWindow());
			m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

			Renderer::Initialize();

			m_IMGUILayer = new IMGUILayer(m_Window);
			PushOverlay(m_IMGUILayer);
		}

		virtual ~Application() = default;

		void OnEvent(Event& event) {
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
				(*--it)->OnEvent(event);
				if (event.Handled) { break; }
			}
		}

		void Run() {
			while (m_Running) {
				float time = (float)glfwGetTime();
				Timestep timestep {
					time - m_LastFrameTime
				};
				m_LastFrameTime = time;


				{
					for (Layer* layer: m_LayerStack) {
						layer->OnUpdate(timestep);
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

		void PushLayer(Layer* layer) {
			m_LayerStack.PushLayer(layer);
			layer->OnAttach();
		}

		void PushOverlay(Layer* overlay) {
			m_LayerStack.PushOverlay(overlay);
			overlay->OnAttach();
		}

		static inline Application& Get() { return *s_Instance; }
		inline IWindow& GetWindow() const { return *m_Window; }
	private:
		static inline Application* s_Instance = nullptr;

		std::shared_ptr<IWindow> m_Window;
		IMGUILayer* m_IMGUILayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float  m_LastFrameTime;

		bool OnWindowClose(WindowCloseEvent& event) {
			m_Running = false;
			return true;
		}
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
