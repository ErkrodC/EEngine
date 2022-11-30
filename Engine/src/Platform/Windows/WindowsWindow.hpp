#pragma once

#include <Renderer/IGraphicsContext.hpp>
#include "IWindow.hpp"
#include "GLFW/glfw3.h"

namespace EEngine {
	class WindowsWindow : public IWindow {
	public:
		explicit WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		void* GetNativeWindow() const override;

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		GLFWwindow* m_Window{};
		IGraphicsContext* m_Context{};

		virtual void Initialize(const WindowProps& props);
		virtual void Shutdown();

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
} // EEngine
