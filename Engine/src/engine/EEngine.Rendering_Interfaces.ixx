export module EEngine.Rendering:Interfaces;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :Buffers;

using namespace EEngine;

export namespace EEngine::Rendering {
	enum class API {
		DirectX,
		OpenGL,
		Vulkan
	};

	constexpr API g_API = API::OpenGL;

	constexpr std::string GetRendererAPIString() {
		if constexpr (g_API == API::OpenGL) {
			return "OpenGL";
		} else if constexpr (g_API == API::DirectX) {
			return "DirectX";
		} else if constexpr (g_API == API::Vulkan) {
			return "Vulkan";
		} else {
			Log::CoreCritical("Unknown rendering API.");
			throw std::runtime_error("Unknown rendering API.");
		}
	}

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
	// Renderer API (API-agnostic interface) - Base class for all renderer APIs
	// ============================================================================
	// Note: This is NOT templated itself, but derived classes are concrete types
	// that use the Shader type alias (which resolves to OpenGLShader, etc.)
	class IRendererAPI {
	public:
		virtual ~IRendererAPI() = default;

		// Pure virtual API control methods
		virtual void Initialize() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Clear(const Math::vec4& color) = 0;
		virtual void DrawIndexed(const Shared<VertexArray>& vertexArray) = 0;

		// Pure virtual factory methods
		virtual Shared<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) = 0;
		virtual Shared<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) = 0;
		virtual Shared<Shader> CreateShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) = 0;
		virtual Shared<Shader> CreateShader(const std::string& path) = 0;
		virtual Shared<VertexArray> CreateVertexArray() = 0;
		virtual Shared<Texture2D> CreateTexture2D(const std::string& path) = 0;
		virtual Shared<Texture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) = 0;

		// Shader caching - declared but not defined (defined in derived classes in :OpenGL partition)
		virtual bool TryGetOrLoadShader(const std::string& name, Shared<Shader>& outShader) = 0;
	};
}
