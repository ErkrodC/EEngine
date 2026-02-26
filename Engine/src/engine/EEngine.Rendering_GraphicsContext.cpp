module;
#include <glad/glad.h>
#include <GLFW/glfw3.h>

module EEngine.Rendering;
import :GraphicsContext;

namespace EEngine::Rendering {
	OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window)
		: m_Window(window)
	{
		Log::Assert(m_Window, "Window was null when creating OpenGLGraphicsContext");

		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		Log::CoreAssert(status, "Failed to initialize GLAD!");

		Log::CoreInfo(
			"OpenGL Info:\n"
			"\tVendor: {0}\n"
			"\tRenderer: {1}\n"
			"\tVersion: {2}",
			(char*)glGetString(GL_VENDOR),
			(char*)glGetString(GL_RENDERER),
			(char*)glGetString(GL_VERSION)
		);
	}

	void OpenGLGraphicsContext::SwapBuffers() const {
		glfwSwapBuffers(m_Window);
	}
}
