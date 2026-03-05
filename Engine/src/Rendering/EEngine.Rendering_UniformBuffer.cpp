module;
#include <glad/glad.h>

module EEngine.Rendering;
import :UniformBuffer;

namespace EEngine::Rendering {
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(OpenGLUniformBuffer&& other) noexcept
		: m_RendererID(std::exchange(other.m_RendererID, 0)) {}

	OpenGLUniformBuffer& OpenGLUniformBuffer::operator=(OpenGLUniformBuffer&& other) noexcept {
		glDeleteBuffers(1, &m_RendererID);
		m_RendererID = std::exchange(other.m_RendererID, 0);
		return *this;
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) const {
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}
}
