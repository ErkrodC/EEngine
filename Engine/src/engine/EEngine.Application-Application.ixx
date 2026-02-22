module;
#include <GLFW/glfw3.h>
#include <Profiling/Profiling.hpp>

export module EEngine.Application:Application;
import EEngine.Core;
import EEngine.Event;
import EEngine.Profiling;
import EEngine.Rendering;
import EEngine.Standard;
import :Layer;
import :IMGUILayer;
import :Input;
import :Window;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Application
	// ============================================================================

	class Application {
	public:
		Application() {
			Log::CoreInfo("Selected Renderer API: {}", Renderer::GetRendererAPIString(Renderer::GetSelectedAPI()));
			Log::CoreAssert(!s_Instance, "Multiple applications created.");

			s_Instance = this;

			m_Window = CreateWindow();
			m_Window->SetEventCallback([this](auto& event) -> void { OnEvent(event); });

			Input::SetWindow(m_Window);

			Renderer::Initialize();
			Renderer2D::Initialize();

			m_IMGUILayer = MakeShared<IMGUILayer>(m_Window);
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

		void PushLayer(Shared<Layer> layer) {
			layer->OnAttach();
			m_LayerStack.PushLayer(std::move(layer));
		}

		void PushOverlay(Shared<Layer> overlay) {
			overlay->OnAttach();
			m_LayerStack.PushOverlay(std::move(overlay));
		}

		static inline Application& Get() { return *s_Instance; }
		inline IWindow& GetWindow() const { return *m_Window; }
	private:
		static inline Application* s_Instance = nullptr;

		Shared<IWindow> m_Window;
		Shared<IMGUILayer> m_IMGUILayer;
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
			Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());

			return true;
		}
	};

	// To be defined by consumer
	Unique<Application> CreateApplication();
}
