module;
#include <glad/glad.h>

module EEngine.Rendering;
import :VertexArray;

namespace EEngine::Rendering {
	static GLenum ShaderDataToOpenGLBaseType(ShaderData type) {
		switch (type) {
			case ShaderData::None:		break;

			case ShaderData::Float:
			case ShaderData::Float2:
			case ShaderData::Float3:
			case ShaderData::Float4:
			case ShaderData::Mat3:
			case ShaderData::Mat4:		return GL_FLOAT;

			case ShaderData::Int:
			case ShaderData::Int2:
			case ShaderData::Int3:
			case ShaderData::Int4:			return GL_INT;

			case ShaderData::Bool:			return GL_BOOL;
		}

		Log::CoreError("Unknown shader data type.");
		return GL_NONE;
	}

	OpenGLVertexArray::OpenGLVertexArray() { glCreateVertexArrays(1, &m_RendererID); }
	OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

	OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray&& other) noexcept
		: m_RendererID(std::exchange(other.m_RendererID, 0)),
		  m_IndexBuffer(std::move(other.m_IndexBuffer)),
		  m_VertexBuffers(std::move(other.m_VertexBuffers)) {}

	OpenGLVertexArray& OpenGLVertexArray::operator=(OpenGLVertexArray&& other) noexcept {
		glDeleteVertexArrays(1, &m_RendererID);
		// ER NOTE: `m_RendererID = 0` protects against double-delete due to self-assignment
		// i.e. enables std::exchange(other.m_RendererID, 0) to return 0, thus leaving this/other in a moved-from
		// state, and that calls glDeleteBuffers(1, &0) (no-op) when destructed.
		m_RendererID = 0;

		m_RendererID = std::exchange(other.m_RendererID, 0);
		m_IndexBuffer = std::move(other.m_IndexBuffer);
		m_VertexBuffers = std::move(other.m_VertexBuffers);

		return *this;
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_RendererID);
		m_IndexBuffer->Bind();
	}

	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) {
		Log::CoreAssert(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout.");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index++,
				(GLint)element.ComponentCount,
				ShaderDataToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				(GLsizei)layout.GetStride(),
				(const void *)(intptr_t)element.Offset
			);
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
}
