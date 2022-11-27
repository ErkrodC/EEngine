#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "OpenGLContext.hpp"

namespace EEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_Window(window) {
		EE_ASSERT(m_Window, "Window was null when creating OpenGLContext");
	}

	void OpenGLContext::Initialize() {
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EE_CORE_ASSERT(status, "Failed to initialize GLAD!");

		EE_CORE_INFO("OpenGL Info:");
		EE_CORE_INFO("\tVendor: {0}", (char*)glGetString(GL_VENDOR));
		EE_CORE_INFO("\tRenderer: {0}", (char*)glGetString(GL_RENDERER));
		EE_CORE_INFO("\tVersion: {0}", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {
		/*glBegin(GL_TRIANGLES);
		glEnd();*/

		glfwSwapBuffers(m_Window);
	}
} // EEngine