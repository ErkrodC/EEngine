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
	}

	void OpenGLContext::SwapBuffers() {
		/*glBegin(GL_TRIANGLES);
		glEnd();*/

		glfwSwapBuffers(m_Window);
	}
} // EEngine