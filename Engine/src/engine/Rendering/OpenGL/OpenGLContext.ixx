module;
#include <GLFW/glfw3.h>
#include <glad/glad.h>

export module EEngine.Rendering:OpenGLContext;
import :IGraphicsContext;
import EEngine.Core;

export namespace EEngine {
	class OpenGLContext : public IGraphicsContext {
	public:
		explicit OpenGLContext(GLFWwindow* window)
			: m_Window(window) {
			Log::Assert(m_Window, "Window was null when creating OpenGLContext");
		}

		void Initialize() override {
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

		void SwapBuffers() override {
			/*glBegin(GL_TRIANGLES);
			glEnd();*/

			glfwSwapBuffers(m_Window);
		}
	private:
		GLFWwindow* m_Window;
	};
} // EEngine
