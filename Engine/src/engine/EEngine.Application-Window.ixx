module;
#include <GLFW/glfw3.h>

export module EEngine.Application:Window;
import EEngine.Core;
import EEngine.Event;
import EEngine.Rendering;
import EEngine.Standard;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Window Interface & Props
	// ============================================================================

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		explicit WindowProps(
			const std::string& title = "EEngine",
			unsigned int width = 1280,
			unsigned int height = 720
		) : Title(title), Width(width), Height(height) {
		}
	};

	class IWindow {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~IWindow() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
	};
}

// ============================================================================
// Platform-specific Implementations (Windows)
// ============================================================================

namespace EEngine {
#if WIN32
	inline int EngineToGLFWKeyCode(KeyCode engineKeyCode) { return (int)engineKeyCode; }
	inline int EngineToGLFWMouseButtonCode(MouseButtonCode engineMouseButtonCode) { return (int)engineMouseButtonCode; }
	inline KeyCode GLFWToEngineKeyCode(int glfwKeyCode) { return (KeyCode)glfwKeyCode; }
	inline MouseButtonCode GLFWToEngineMouseButtonCode(int glfwMouseButtonCode) { return (MouseButtonCode)glfwMouseButtonCode; }
#endif
}

export namespace EEngine {
	class WindowsWindow : public IWindow {
	public:
		explicit WindowsWindow(const WindowProps& props) {
			Initialize(props);
		}

		~WindowsWindow() override {
			Shutdown();
		}

		void OnUpdate() override {
			glfwPollEvents();
			if (m_Context) { m_Context->SwapBuffers(); }
		}

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		void* GetNativeWindow() const override {
			return m_Window;
		}

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled) override {
			if (enabled) {
				glfwSwapInterval(1);
			} else {
				glfwSwapInterval(0);
			}

			m_Data.VSync = enabled;
		}

		bool IsVSync() const override {
			return m_Data.VSync;
		}
	private:
		inline static bool s_GLFWInitialized = false;

		GLFWwindow* m_Window{};
		IGraphicsContext* m_Context{};

		virtual void Initialize(const WindowProps& props) {
			m_Data.Title = props.Title;
			m_Data.Width = props.Width;
			m_Data.Height = props.Height;

			if (!s_GLFWInitialized) {
				int success = glfwInit();
				Log::CoreAssert(success, "Could not initialize GLFW!");
				glfwSetErrorCallback([](int error, const char* description) {
					Log::CoreError("GLFW Error {0}: {1}", error, description);
				});

				s_GLFWInitialized = true;
			}

			m_Window = glfwCreateWindow(
				(int)props.Width,
				(int)props.Height,
				m_Data.Title.c_str(),
				nullptr,
				nullptr
			);

			m_Context = new OpenGLContext(m_Window);
			m_Context->Initialize();
			Log::CoreInfo("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

			glfwSetWindowUserPointer(m_Window, &m_Data);
			SetVSync(true);

			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;
				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int glfwKeyCode, int, int action, int) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyCode engineKeyCode = GLFWToEngineKeyCode(glfwKeyCode);
				switch (action) {
					case GLFW_PRESS: {
						KeyPressedEvent event(engineKeyCode, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE: {
						KeyReleasedEvent event(engineKeyCode);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT: {
						KeyPressedEvent event(engineKeyCode, 1);
						data.EventCallback(event);
						break;
					}
					default: break;
				}
			});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int glfwKeyCode) {
				WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
				KeyTypedEvent event(GLFWToEngineKeyCode((int)glfwKeyCode));
				data.EventCallback(event);
			});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int glfwMouseButtonCode, int action, int ) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseButtonCode engineMouseButtonCode = GLFWToEngineMouseButtonCode(glfwMouseButtonCode);
				switch (action) {
					case GLFW_PRESS: {
						MouseButtonPressedEvent event(engineMouseButtonCode);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE: {
						MouseButtonReleasedEvent event(engineMouseButtonCode);
						data.EventCallback(event);
						break;
					}
					default: break;
				}
			});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
		}

		virtual void Shutdown() {
			glfwDestroyWindow(m_Window);
		}

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

// ============================================================================
// Window Creation (TBD namespace for platform abstraction)
// ============================================================================

export namespace EEngine::TBD {
	IWindow* CreateWindow(const WindowProps& props = WindowProps()) {
		return new WindowsWindow(props);
	}
}
