module;
#include <glad/glad.h>

export module EEngine.Rendering:VertexBuffer;
import EEngine.Core;
import EEngine.Standard;
import :API;
import :Buffers;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLVertexBuffer Implementation
	// ============================================================================
	class OpenGLVertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size) {
			glCreateBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }
		OpenGLVertexBuffer(const OpenGLVertexBuffer& other) = delete;
		OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer& other) = delete;

		OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept
			: m_RendererID(std::exchange(other.m_RendererID, 0)),
			m_Layout(std::move(other.m_Layout)) {}

		OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& other) noexcept {
			glDeleteBuffers(1, &m_RendererID);
			m_RendererID = 0;

			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_Layout = std::move(other.m_Layout);

			return *this;
		}

		void Bind() const  {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		}

		void Unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		const BufferLayout& GetLayout() const  {
			return m_Layout;
		}

		void SetLayout(const BufferLayout& layout) {
			m_Layout = layout;
		}
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
	concept VertexBufferConcept = requires(T buffer, BufferLayout layout) {
		{ buffer.Bind() } -> std::same_as<void>;
		{ buffer.Unbind() } -> std::same_as<void>;
		{ buffer.GetLayout() } -> std::same_as<const BufferLayout&>;
		{ buffer.SetLayout(layout) } -> std::same_as<void>;
	};
	static_assert(VertexBufferConcept<VertexBuffer>);
}
