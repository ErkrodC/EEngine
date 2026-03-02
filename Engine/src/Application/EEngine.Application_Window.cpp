module;
#include <GLFW/glfw3.h>

module EEngine.Application;
import :Window;

namespace EEngine {
	void WindowsWindow::GLFWWindowDeleter::operator()(GLFWwindow* window) const {
		if (window) { glfwDestroyWindow(window); }
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
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

		m_Window = MakeUniqueFromRaw(glfwCreateWindow(
			static_cast<int>(props.Width),
			static_cast<int>(props.Height),
			m_Data.Title.c_str(),
			nullptr,
			nullptr
		), GLFWWindowDeleter{});

		m_Context = MakeUnique<GraphicsContext>(m_Window.get());
		Log::CoreInfo("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		glfwSetWindowUserPointer(m_Window.get(), &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Width = static_cast<uint32_t>(width);
			data.Height = static_cast<uint32_t>(height);
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window.get(), [](GLFWwindow* window, int glfwKeyCode, int, int action, int) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyCode engineKeyCode = Windows::GLFWToEngineKeyCode(glfwKeyCode);
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

		glfwSetCharCallback(m_Window.get(), [](GLFWwindow* window, unsigned int glfwKeyCode) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(Windows::GLFWToEngineKeyCode(glfwKeyCode));
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window.get(), [](GLFWwindow* window, int glfwMouseButtonCode, int action, int ) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseButtonCode engineMouseButtonCode = Windows::GLFWToEngineMouseButtonCode(glfwMouseButtonCode);
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

		glfwSetScrollCallback(m_Window.get(), [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseScrolledEvent event(static_cast<float_t>(xOffset), static_cast<float_t>(yOffset));
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window.get(), [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseMovedEvent event(static_cast<float_t>(xPos), static_cast<float_t>(yPos));
			data.EventCallback(event);
		});
	}

	void WindowsWindow::OnUpdate() const {
		glfwPollEvents();
		if (m_Context) { m_Context->SwapBuffers(); }
	}

	void WindowsWindow::SetVSync(bool enabled) {
		glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	double_t WindowsWindow::GetTime() const {
		return glfwGetTime();
	}
}
