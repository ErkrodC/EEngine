#include "WindowsWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Input.hpp"
#include <Platform/OpenGL/OpenGLContext.hpp>

namespace EEngine {
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const EEngine::WindowProps& props) {
		Initialize(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Initialize(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		EE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			EE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback([](int error, const char* description) {
				EE_CORE_ERROR("GLFW Error {0}: {1}", error, description);
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

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
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

			KeyCode engineKeyCode = Input::GLFWToEngineKeyCode(glfwKeyCode);
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
			KeyTypedEvent event(Input::GLFWToEngineKeyCode((int)glfwKeyCode));
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int glfwMouseButtonCode, int action, int ) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseButtonCode engineMouseButtonCode = Input::GLFWToEngineMouseButtonCode(glfwMouseButtonCode);
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

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		if (m_Context) { m_Context->SwapBuffers(); }
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

	void* WindowsWindow::GetNativeWindow() const {
		return m_Window;
	}
} // EEngine