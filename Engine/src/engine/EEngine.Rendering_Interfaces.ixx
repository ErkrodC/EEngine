export module EEngine.Rendering:Interfaces;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :Buffers;

using namespace EEngine;

export namespace EEngine {
	enum class API {
		DirectX,
		OpenGL,
		Vulkan
	};

	constexpr API g_API = API::OpenGL;

	#define DECLARE_API_TYPE(BaseName) \
		class DirectX##BaseName; class OpenGL##BaseName; class Vulkan##BaseName; \
		using BaseName = std::conditional_t<g_API == API::DirectX, \
			DirectX##BaseName, \
			std::conditional_t<g_API == API::OpenGL, \
				OpenGL##BaseName, \
				Vulkan##BaseName \
			> \
		>

	DECLARE_API_TYPE(Shader);
	DECLARE_API_TYPE(VertexArray);
	DECLARE_API_TYPE(Texture2D);
	DECLARE_API_TYPE(IndexBuffer);
	DECLARE_API_TYPE(VertexBuffer);

	#undef DECLARE_API_TYPE

	// ============================================================================
	// Graphics Context (API-agnostic interface)
	// ============================================================================
	class IGraphicsContext {
	public:
		virtual void SwapBuffers() = 0;
	};

	// ============================================================================
	// Renderer API (API-agnostic interface)
	// ============================================================================
	class IRendererAPI {
	public:
		virtual void InitializeImpl() = 0;
		virtual void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void ClearImpl(const Math::vec4& color) = 0;
		virtual void DrawIndexedImpl(const Shared<VertexArray>& vertexArray) = 0;
		virtual Shared<IndexBuffer> CreateIndexBufferImpl(uint32_t* indices, uint32_t count) = 0;
		virtual Shared<VertexBuffer> CreateVertexBufferImpl(float* vertices, uint32_t size) = 0;
		virtual Shared<Shader> CreateShaderImpl(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) = 0;
		virtual Shared<Shader> CreateShaderImpl(const std::string& path) = 0;
		virtual Shared<VertexArray> CreateVertexArrayImpl() = 0;
		virtual Shared<Texture2D> CreateTexture2DImpl(const std::string& path) = 0;
		virtual Shared<Texture2D> CreateTexture2DImpl(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) = 0;
	};
}
