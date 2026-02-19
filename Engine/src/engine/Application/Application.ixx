module;
#include <GLFW/glfw3.h>

export module EEngine.Application:Application;
import :IInput;
import :IMGUILayer;
import :IWindow;
import :Layer;
import :LayerStack;
import :TBD;
import EEngine.Core;
import EEngine.Event;
import EEngine.Profiling;
import EEngine.Rendering;
import EEngine.Standard;

export namespace EEngine {
	class Application {
	public:
		Application() {
			Log::CoreInfo("Selected Renderer API: {}", Renderer::GetRendererAPIString(Renderer::GetSelectedAPI()));
			Log::CoreAssert(!s_Instance, "Multiple applications created.");

			s_Instance = this;

			m_Window.reset(TBD::CreateWindow());
			m_Window->SetEventCallback([this](auto& event) -> void { OnEvent(event); });

			Renderer::Initialize();
			Renderer2D::Initialize();

			m_IMGUILayer = new IMGUILayer(m_Window);
			PushOverlay(m_IMGUILayer);
		}

		virtual ~Application() = default;

		void OnEvent(Event& event) {
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<WindowCloseEvent>([this](auto& event) -> bool { return OnWindowClose(event); });
			dispatcher.Dispatch<WindowResizeEvent>([this](auto& event) -> bool { return OnWindowResized(event); });

			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
				(*--it)->OnEvent(event);
				if (event.Handled) { break; }
			}
		}

		void Run() {
			while (m_Running) {
				Profiling::Profiler::Get().BeginFrame();

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

				Profiling::Profiler::Get().EndFrame();
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
				Log::CoreTrace("{} {} {} {} {} {} {} {}",
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse1),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse2),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse3),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse4),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse5),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse6),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse7),
					Input::IsMouseButtonPressed(MouseButtonCode::Mouse8)
				);
				Log::CoreTrace("Resized to ({}, {})", event.GetWidth(), event.GetHeight());
				Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());
			}*/

			Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());

			return true;
		}
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
