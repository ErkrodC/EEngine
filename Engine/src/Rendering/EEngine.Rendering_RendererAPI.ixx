export module EEngine.Rendering:RendererAPI;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :API;
import :IndexBuffer;
import :VertexBuffer;
import :VertexArray;
import :UniformBuffer;
import :Shader;
import :Texture;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLRendererAPI Implementation
	// ============================================================================
	class OpenGLRendererAPI {
	public:
		void Initialize() const;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
		void Clear(const Math::vec4& color) const;
		void DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCountOverride = 0) const;

		// Shadow map frame buffer
		void CreateShadowMap(uint32_t resolution);
		void BindShadowFrameBuffer() const;
		void UnbindShadowFrameBuffer() const;
		void BindShadowTexture(uint32_t count) const;
		uint32_t GetShadowMapResolution() const { return m_ShadowMapResolution; }

		// HDR frame buffer for post-processing
		void CreateHDRFrameBuffer(uint32_t width, uint32_t height);
		void ResizeHDRFrameBuffer(uint32_t width, uint32_t height);
		void BindHDRFrameBuffer() const;
		void UnbindHDRFrameBuffer() const;
		void BindHDRTexture(uint32_t slot) const;

		Shared<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) const;
		Shared<VertexBuffer> CreateVertexBuffer(void* vertices, uint32_t size) const;
		Shared<Shader> CreateShader(const std::string& path) const;
		Shared<Shader> CreateShader(
			const std::string& name,
			const std::string& vertexSource,
			const std::string& fragmentSource
		) const;
		Shared<VertexArray> CreateVertexArray() const;
		Shared<UniformBuffer> CreateUniformBuffer(uint32_t size, uint32_t binding) const;
		Shared<Texture2D> CreateTexture2D(const std::string& path) const;
		Shared<Texture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data, uint32_t size) const;

		bool TryGetOrLoadShader(const std::string& name, Shared<Shader>& outShader);

	private:
		std::unordered_map<std::string, Shared<Shader>> m_ShaderByName;
		uint32_t m_ShadowFBO = 0;
		uint32_t m_ShadowDepthTexture = 0;
		uint32_t m_ShadowMapResolution = 0;

		uint32_t m_HDRFBO = 0;
		uint32_t m_HDRColorTexture = 0;
		uint32_t m_HDRDepthRBO = 0;
		uint32_t m_HDRWidth = 0;
		uint32_t m_HDRHeight = 0;

		Shared<Shader> CreateAndCacheShader(const std::string& path);
	};

	// ============================================================================
	// RendererAPI Type Alias
	// ============================================================================
	class DirectXRendererAPI; 
	class VulkanRendererAPI; 
	using RendererAPI = std::conditional_t<g_API == API::DirectX, 
		DirectXRendererAPI, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLRendererAPI, 
			VulkanRendererAPI 
		> 
	>;

	// ============================================================================
	// RendererAPI Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept RendererAPIConcept = requires(
		T api,
		uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t size, uint32_t count,
		uint32_t* indices,
		QuadVertex* vertices,
		Math::vec4 color,
		std::string path, std::string name, std::string vertexSource, std::string fragmentSource,
		Shared<Shader> shader
	) {
		{ api.Initialize() } -> std::same_as<void>;
		{ api.SetViewport(x, y, x, y) } -> std::same_as<void>;
		{ api.Clear(color) } -> std::same_as<void>;
		{ api.DrawIndexed(Shared<VertexArray>{}) } -> std::same_as<void>;

		{ api.CreateShadowMap(size) } -> std::same_as<void>;
		{ api.BindShadowFrameBuffer() } -> std::same_as<void>;
		{ api.UnbindShadowFrameBuffer() } -> std::same_as<void>;
		{ api.BindShadowTexture(count) } -> std::same_as<void>;
		{ api.GetShadowMapResolution() } -> std::same_as<uint32_t>;

		{ api.CreateHDRFrameBuffer(width, height) } -> std::same_as<void>;
		{ api.ResizeHDRFrameBuffer(width, height) } -> std::same_as<void>;
		{ api.BindHDRFrameBuffer() } -> std::same_as<void>;
		{ api.UnbindHDRFrameBuffer() } -> std::same_as<void>;
		{ api.BindHDRTexture(count) } -> std::same_as<void>;

		{ api.CreateIndexBuffer(indices, count) } -> std::same_as<Shared<IndexBuffer>>;
		{ api.CreateVertexBuffer(vertices, size) } -> std::same_as<Shared<VertexBuffer>>;
		{ api.CreateShader(name, vertexSource, fragmentSource) } -> std::same_as<Shared<Shader>>;
		{ api.CreateShader(path) } -> std::same_as<Shared<Shader>>;
		{ api.CreateVertexArray() } -> std::same_as<Shared<VertexArray>>;
		{ api.CreateUniformBuffer(size, count) } -> std::same_as<Shared<UniformBuffer>>;
		{ api.CreateTexture2D(path) } -> std::same_as<Shared<Texture2D>>;
		{ api.CreateTexture2D(width, height, vertices, size) } -> std::same_as<Shared<Texture2D>>;
		{ api.TryGetOrLoadShader(path, shader) } -> std::same_as<bool>;
	};
	static_assert(RendererAPIConcept<RendererAPI>);
}
