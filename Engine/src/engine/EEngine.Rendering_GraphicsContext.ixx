module;
#include <glad/glad.h>
#include <GLFW/glfw3.h>

export module EEngine.Rendering:GraphicsContext;
import EEngine.Core;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLGraphicsContext Implementation
	// ============================================================================
	class OpenGLGraphicsContext {
	public:
		explicit OpenGLGraphicsContext(GLFWwindow* window)
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

		void SwapBuffers() const {
			glfwSwapBuffers(m_Window);
		}
	private:
		// non-owning, window guaranteed to outlive context because it is declared before
		// (thus destroyed after) the IGraphicsContext member of class WindowsWindow
		GLFWwindow* m_Window;
	};

	// ============================================================================
	// GraphicsContext Type Alias
	// ============================================================================
	class DirectXGraphicsContext; 
	class VulkanGraphicsContext; 
	using GraphicsContext = std::conditional_t<g_API == API::DirectX, 
		DirectXGraphicsContext, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLGraphicsContext, 
			VulkanGraphicsContext 
		> 
	>;

	// ============================================================================
	// GraphicsContext Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept GraphicsContextConcept = requires(T context) {
		{ context.SwapBuffers() } -> std::same_as<void>;
	};
	static_assert(GraphicsContextConcept<GraphicsContext>);
}
