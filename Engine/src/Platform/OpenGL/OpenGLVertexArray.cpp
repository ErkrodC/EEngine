#include <glad/glad.h>
#include "OpenGLVertexArray.hpp"

namespace EEngine {
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

		EE_CORE_ERROR("Unknown shader data type.");
		return GL_NONE;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) {
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) {
		EE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout.");

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
} // EEngine