module;
#include <glad/glad.h>

module EEngine.Rendering;
import :IndexBuffer;

namespace EEngine::Rendering {
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

	OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept
		: m_RendererID(std::exchange(other.m_RendererID, 0)),
		  m_Count(std::move(other.m_Count)) { }

	OpenGLIndexBuffer& OpenGLIndexBuffer::operator=(OpenGLIndexBuffer&& other) noexcept {
		glDeleteBuffers(1, &m_RendererID);
		// ER NOTE: `m_RendererID = 0` protects against double-delete due to self-assignment
		// i.e. enables std::exchange(other.m_RendererID, 0) to return 0, thus leaving this/other in a moved-from
		// state, and that calls glDeleteBuffers(1, &0) (no-op) when destructed.
		m_RendererID = 0;

		m_RendererID = std::exchange(other.m_RendererID, 0);
		m_Count = std::move(other.m_Count);

		return *this;
	}

	void OpenGLIndexBuffer::Bind() const  {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
