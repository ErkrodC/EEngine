module;
#include "GLFW_fwd.hpp"

export module EEngine.Application:Window;

import EEngine.Core;
import EEngine.Event;
import EEngine.Rendering;
import EEngine.Standard;

using namespace EEngine;
using namespace Rendering;

namespace EEngine {
	using EventCallbackFn = std::function<void(Event&)>;

	// ============================================================================
	// WindowProps Struct
	// ============================================================================
	export struct WindowProps {
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		explicit WindowProps(
			const std::string& title = "EEngine",
			uint32_t width = 1280,
			uint32_t height = 720
		) : Title(title), Width(width), Height(height) {}
	};

	// ============================================================================
	// WindowsWindow Implementation
	// ============================================================================
	export class WindowsWindow {
		// Custom deleter for GLFWwindow (opaque type)
		struct GLFWWindowDeleter {
			void operator()(GLFWwindow* window) const;
		};

	public:
		explicit WindowsWindow(const WindowProps& props);

		void OnUpdate() const;

		inline uint32_t GetWidth() const { return m_Data.Width; }
		inline uint32_t GetHeight() const { return m_Data.Height; }

		void* GetNativeWindow() const { return m_Window.get(); }

		inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

		bool IsVSync() const { return m_Data.VSync; }
		void SetVSync(bool enabled);

		double_t GetTime() const;

	private:
		static inline bool s_GLFWInitialized = false;

		UniqueD<GLFWwindow, GLFWWindowDeleter> m_Window;
		Unique<GraphicsContext> m_Context;

		struct WindowData {
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

	// ============================================================================
	// Window Concept, Alias, and Static Assert
	// ============================================================================
	class MacWindow; class LinuxWindow;
	export using Window = std::conditional_t<g_Platform == Platform::MacOS,
		MacWindow,
		std::conditional_t<g_Platform == Platform::Linux,
			LinuxWindow,
			WindowsWindow
		>
	>;

	export template<typename TWindow>
	concept WindowConcept = requires(
		TWindow window,
		WindowProps& props,
		EventCallbackFn& callback,
		bool enabled
	) {
		{ TWindow(props) } -> std::same_as<TWindow>;
		{ window.OnUpdate() } -> std::same_as<void>;
		{ window.GetWidth() } -> std::same_as<uint32_t>;
		{ window.GetHeight() } -> std::same_as<uint32_t>;
		{ window.SetEventCallback(callback) } -> std::same_as<void>;
		{ window.IsVSync() } -> std::same_as<bool>;
		{ window.SetVSync(enabled) } -> std::same_as<void>;
		{ window.GetNativeWindow() } -> std::same_as<void*>;
		{ window.GetTime() } -> std::same_as<double_t>;
	};
	static_assert(WindowConcept<Window>);
}
