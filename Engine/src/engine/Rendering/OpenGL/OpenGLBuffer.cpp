module;
#include <cstdint>
#include <glad/glad.h>

module EEngine.Rendering:OpenGLBuffer;
import :Buffer;

namespace EEngine {
	// Index Buffer-------------------------------------------------------------------------------------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(
		uint32_t* indices,
		uint32_t count
	) : m_Count(count) {
		glCreateBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Vertex Buffer-------------------------------------------------------------------------------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(
		float* vertices,
		uint32_t size
	) {
		glCreateBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const BufferLayout& OpenGLVertexBuffer::GetLayout() const {
		return m_Layout;
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) {
		m_Layout = layout;/*

		auto& elements = layout.GetElements();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, elements.size(), GL_FLOAT, GL_FALSE, element.Size, ())*/
	}
} // EEngine