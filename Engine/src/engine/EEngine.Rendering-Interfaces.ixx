export module EEngine.Rendering:Interfaces;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :Buffers;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Shader (API-agnostic interface)
	// ============================================================================

	class IShader {
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int32_t value) = 0;

		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const Math::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const Math::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const Math::vec4& value) = 0;

		virtual void SetMat3(const std::string& name, const Math::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const Math::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;
	};

	// ============================================================================
	// Texture (API-agnostic interface)
	// ============================================================================

	class ITexture {
	public:
		virtual ~ITexture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind() const = 0;
	};

	class ITexture2D : public ITexture { };

	// ============================================================================
	// Vertex Array (API-agnostic interface)
	// ============================================================================

	class IVertexArray {
	public:
		virtual ~IVertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetIndexBuffer(const Shared<IIndexBuffer>& indexBuffer) = 0;
		virtual void AddVertexBuffer(const Shared<IVertexBuffer>& vertexBuffer) = 0;

		virtual const Shared<IIndexBuffer>& GetIndexBuffer() const = 0;
		virtual const std::vector<Shared<IVertexBuffer>>& GetVertexBuffers() const = 0;
	};

	// ============================================================================
	// Graphics Context (API-agnostic interface)
	// ============================================================================

	class IGraphicsContext {
	public:
		virtual void Initialize() = 0;
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
		virtual void DrawIndexedImpl(const Shared<IVertexArray>& vertexArray) = 0;
		virtual Shared<IIndexBuffer> CreateIndexBufferImpl(uint32_t* indices, uint32_t count) = 0;
		virtual Shared<IVertexBuffer> CreateVertexBufferImpl(float* vertices, uint32_t size) = 0;
		virtual Shared<IShader> CreateShaderImpl(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) = 0;
		virtual Shared<IShader> CreateShaderImpl(const std::string& path) = 0;
		virtual Shared<IVertexArray> CreateVertexArrayImpl() = 0;
		virtual Shared<ITexture2D> CreateTexture2DImpl(const std::string& path) = 0;
		virtual Shared<ITexture2D> CreateTexture2DImpl(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) = 0;
	};
}
