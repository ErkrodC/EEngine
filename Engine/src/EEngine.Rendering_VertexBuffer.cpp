module;
#include <glad/glad.h>

module EEngine.Rendering;
import :VertexBuffer;

namespace EEngine::Rendering {
	OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size) {
		glCreateBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

	OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept
		: m_RendererID(std::exchange(other.m_RendererID, 0)),
		m_Layout(std::move(other.m_Layout)) {}

	OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(OpenGLVertexBuffer&& other) noexcept {
		glDeleteBuffers(1, &m_RendererID);
		// ER NOTE: `m_RendererID = 0` protects against double-delete due to self-assignment
		// i.e. enables std::exchange(other.m_RendererID, 0) to return 0, thus leaving this/other in a moved-from
		// state, and that calls glDeleteBuffers(1, &0) (no-op) when destructed.
		m_RendererID = 0;

		m_RendererID = std::exchange(other.m_RendererID, 0);
		m_Layout = std::move(other.m_Layout);

		return *this;
	}

	void OpenGLVertexBuffer::Bind() const  {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
		glNamedBufferSubData(m_RendererID, 0, size, data);
	}
}
