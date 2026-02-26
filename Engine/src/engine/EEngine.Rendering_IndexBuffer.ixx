module;
#include <glad/glad.h>

export module EEngine.Rendering:IndexBuffer;
import EEngine.Core;
import EEngine.Standard;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLIndexBuffer Implementation
	// ============================================================================
	class OpenGLIndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
			: m_Count(count)
		{
			glCreateBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		}

		~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }
		OpenGLIndexBuffer(const OpenGLIndexBuffer& other) = delete;
		OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer& other) = delete;

		OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept
			: m_RendererID(std::exchange(other.m_RendererID, 0)),
			  m_Count(std::move(other.m_Count)) { }

		OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& other) noexcept {
			glDeleteBuffers(1, &m_RendererID);
			// ER NOTE: `m_RendererID = 0` protects against double-delete due to self-assignment
			// i.e. enables std::exchange(other.m_RendererID, 0) to return 0, thus leaving this/other in a moved-from
			// state, and that calls glDeleteBuffers(1, &0) (no-op) when destructed.
			m_RendererID = 0;

			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_Count = std::move(other.m_Count);

			return *this;
		}

		void Bind() const  {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		}

		void Unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	// ============================================================================
	// IndexBuffer Type Alias
	// ============================================================================
	class DirectXIndexBuffer; 
	class VulkanIndexBuffer; 
	using IndexBuffer = std::conditional_t<g_API == API::DirectX, 
		DirectXIndexBuffer, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLIndexBuffer, 
			VulkanIndexBuffer 
		> 
	>;

	// ============================================================================
	// IndexBuffer Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept IndexBufferConcept = requires(T buffer) {
		{ buffer.Bind() } -> std::same_as<void>;
		{ buffer.Unbind() } -> std::same_as<void>;
		{ buffer.GetCount() } -> std::same_as<uint32_t>;
	};
	static_assert(IndexBufferConcept<IndexBuffer>);
}
