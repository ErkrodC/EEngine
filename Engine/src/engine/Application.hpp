#pragma once

#include <Renderer/Shader.hpp>
#include <Renderer/Buffer.hpp>
#include <Renderer/IVertexArray.hpp>
#include <Renderer/Camera.hpp>
#include "Core.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "IWindow.hpp"
#include "imgui/IMGUILayer.hpp"

namespace EEngine {
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
		static Application* s_Instance;

		std::unique_ptr<IWindow> m_Window;
		IMGUILayer* m_IMGUILayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& event);
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
