export module EEngine.Rendering:RendererAPI;
import EEngine.Standard;
import :Interfaces;
import :OpenGL;

export namespace EEngine::Rendering {
	// ============================================================================
	// RendererAPI Type Alias (compile-time API selection),
	// Use IRendererAPI to avoid needing to import implementations, where used
	// ============================================================================

	// ER TODO: remove forward declarations of RendererAPI implementation and import their partition (e.g. import :Vulkan)
	// if/when those are written
	class DirectXRendererAPI; class VulkanRendererAPI;
	using RendererAPI = std::conditional_t<g_API == API::DirectX,
		DirectXRendererAPI,
		std::conditional_t<g_API == API::OpenGL,
			OpenGLRendererAPI,
			VulkanRendererAPI
		>
	>;

	class DirectXContext; class VulkanContext;
	using GraphicsContext = std::conditional_t<g_API == API::DirectX,
		DirectXContext,
		std::conditional_t<g_API == API::OpenGL,
			OpenGLContext,
			VulkanContext
		>
	>;
}