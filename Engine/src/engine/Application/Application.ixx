module;
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
import EEngine.std.core;

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
			dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));

			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
				(*--it)->OnEvent(event);
				if (event.Handled) { break; }
			}
		}

		void Run() {
			while (m_Running) {
				auto time = (float)glfwGetTime();
				Timestep timestep {
					time - m_LastFrameTime
				};
				m_LastFrameTime = time;


				if (!m_Minimized) {
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
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float  m_LastFrameTime;

		bool OnWindowClose(WindowCloseEvent& event) {
			m_Running = false;
			return true;
		}

		bool OnWindowResized(WindowResizeEvent& event) {
			if (event.GetWidth() == 0 || event.GetHeight() == 0) {
				m_Minimized = true;
				return false;
			}

			m_Minimized = false;
			// ER TODO do resize only on mouse up/drag end.
			// no mouse button event for dragging window edge tho...
			/*if (!Input::IsMouseButtonPressed(MouseButtonCode::Mouse1)) {
				EE_CORE_TRACE("{} {} {} {} {} {} {} {}",
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse1),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse2),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse3),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse4),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse5),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse6),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse7),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse8)
				);
				EE_CORE_TRACE("Resized to ({}, {})", event.GetWidth(), event.GetHeight());
				Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());
			}*/

			Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());

			return true;
		}
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
