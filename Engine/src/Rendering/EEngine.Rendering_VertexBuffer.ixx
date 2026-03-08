export module EEngine.Rendering:VertexBuffer;
import EEngine.Core;
import EEngine.Standard;
import EEngine.Math;
import :API;
import :Buffers;

export namespace EEngine::Rendering {
	struct Vertex {
		Math::vec4 Color;
		Math::vec3 Normal;
		Math::vec3 Position;
		Math::vec2 TexCoord;
	};

	struct QuadVertex {
		Math::vec4 Color;
		Math::vec3 Position;
		Math::vec2 TexCoord;
	};

	// ============================================================================
	// OpenGLVertexBuffer Implementation
	// ============================================================================
	class OpenGLVertexBuffer {
	public:
		OpenGLVertexBuffer(void* vertices, uint32_t size);
		~OpenGLVertexBuffer();
		OpenGLVertexBuffer(const OpenGLVertexBuffer& other) = delete;
		OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer& other) = delete;

		OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept;
		OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& other) noexcept;

		void Bind() const;
		void Unbind() const;

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

		void SetData(const void* data, uint32_t size);
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout{};
	};

	// ============================================================================
	// VertexBuffer Type Alias
	// ============================================================================
	class DirectXVertexBuffer; 
	class VulkanVertexBuffer; 
	using VertexBuffer = std::conditional_t<g_API == API::DirectX, 
		DirectXVertexBuffer, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLVertexBuffer, 
			VulkanVertexBuffer 
		> 
	>;

	// ============================================================================
	// VertexBuffer Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept VertexBufferConcept = requires(T buffer, BufferLayout layout, const void* data, uint32_t size) {
		{ buffer.Bind() } -> std::same_as<void>;
		{ buffer.Unbind() } -> std::same_as<void>;
		{ buffer.GetLayout() } -> std::same_as<const BufferLayout&>;
		{ buffer.SetLayout(layout) } -> std::same_as<void>;
		{ buffer.SetData(data, size) } -> std::same_as<void>;
	};
	static_assert(VertexBufferConcept<VertexBuffer>);
}
