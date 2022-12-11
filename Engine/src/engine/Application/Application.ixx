module;
#include <memory>

export module EEngine.Application:Application;
import :IMGUILayer;
import :IWindow;
import :Layer;
import :LayerStack;
import EEngine.Core;
import EEngine.Event;

export namespace EEngine {
	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline IWindow& GetWindow() const { return *m_Window; }
	private:
		static inline Application* s_Instance = nullptr;

		std::unique_ptr<IWindow> m_Window;
		IMGUILayer* m_IMGUILayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float  m_LastFrameTime;

		bool OnWindowClose(WindowCloseEvent& event);
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
