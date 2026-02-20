module;

#include <glad/glad.h>

export module EEngine.Rendering:OpenGLVertexArray;
import :Buffer;
import :IVertexArray;
import EEngine.Core;
import EEngine.Standard;

export namespace EEngine {
	class OpenGLVertexArray : public IVertexArray {
	public:
		OpenGLVertexArray() {
			glCreateVertexArrays(1, &m_RendererID);
		}

		~OpenGLVertexArray() override {
			glDeleteVertexArrays(1, &m_RendererID);
		}

		void Bind() const override {
			glBindVertexArray(m_RendererID);
		}

		void Unbind() const override {
			glBindVertexArray(0);
		}

		void SetIndexBuffer(const Shared<IIndexBuffer>& indexBuffer) override {
			glBindVertexArray(m_RendererID);
			indexBuffer->Bind();

			m_IndexBuffer = indexBuffer;
		}

		void AddVertexBuffer(const Shared<IVertexBuffer>& vertexBuffer) override {
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

		const Shared<IIndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		const std::vector<Shared<IVertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
	private:
		uint32_t m_RendererID;
		Shared<IIndexBuffer> m_IndexBuffer;
		std::vector<Shared<IVertexBuffer>> m_VertexBuffers;

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
	};
} // EEngine
