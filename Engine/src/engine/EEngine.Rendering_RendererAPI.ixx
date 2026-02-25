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

	template<typename T>
	concept RendererAPIConcept = requires(
		T api,
		uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t count, uint32_t size, uint32_t* indices,
		float* vertices,
		Math::vec4 color,
		std::string path, std::string name, std::string vertexSource, std::string fragmentSource,
		Shared<Shader> shader
	) {
		// Control methods - must return void
		{ api.Initialize() } -> std::same_as<void>;
		{ api.SetViewport(x, y, x, y) } -> std::same_as<void>;
		{ api.Clear(color) } -> std::same_as<void>;
		{ api.DrawIndexed(Shared<VertexArray>{}) } -> std::same_as<void>;

		// Factory methods - must return the right types
		{ api.CreateIndexBuffer(indices, count) } -> std::same_as<Shared<IndexBuffer>>;
		{ api.CreateVertexBuffer(vertices, size) } -> std::same_as<Shared<VertexBuffer>>;
		{ api.CreateShader(name, vertexSource, fragmentSource) } -> std::same_as<Shared<Shader>>;
		{ api.CreateShader(path) } -> std::same_as<Shared<Shader>>;
		{ api.CreateVertexArray() } -> std::same_as<Shared<VertexArray>>;
		{ api.CreateTexture2D(path) } -> std::same_as<Shared<Texture2D>>;
		{ api.CreateTexture2D(width, height, vertices, size) } -> std::same_as<Shared<Texture2D>>;

		// Shader caching
		{ api.TryGetOrLoadShader(path, shader) } -> std::same_as<bool>;
	};
	static_assert(RendererAPIConcept<RendererAPI>);


	class DirectXContext; class VulkanContext;
	using GraphicsContext = std::conditional_t<g_API == API::DirectX,
		DirectXContext,
		std::conditional_t<g_API == API::OpenGL,
			OpenGLContext,
			VulkanContext
		>
	>;

	template<typename T>
	concept GraphicsContextConcept = requires(T context) {
		{ context.SwapBuffers() } -> std::same_as<void>;
	};

	static_assert(GraphicsContextConcept<GraphicsContext>);
}