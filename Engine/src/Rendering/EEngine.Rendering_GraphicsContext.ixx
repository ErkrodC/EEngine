module;
#include "../GLFW_fwd.hpp"

export module EEngine.Rendering:GraphicsContext;
import EEngine.Core;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLGraphicsContext Implementation
	// ============================================================================
	class OpenGLGraphicsContext {
	public:
		explicit OpenGLGraphicsContext(GLFWwindow* window);

		void SwapBuffers() const;
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
