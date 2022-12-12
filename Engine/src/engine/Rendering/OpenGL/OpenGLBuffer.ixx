module;
#include <glad/glad.h>

export module EEngine.Rendering:OpenGLBuffer;
import :Buffer;
import EEngine.std.core;

export namespace EEngine {
	class OpenGLIndexBuffer : public IIndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
			: m_Count(count) {
			glCreateBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		}

		~OpenGLIndexBuffer() override {
			glDeleteBuffers(1, &m_RendererID);
		}

		void Bind() const override {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		}

		void Unbind() const override {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLVertexBuffer : public IVertexBuffer {
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout{};

	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size) {
			glCreateBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		~OpenGLVertexBuffer() override {
			glDeleteBuffers(1, &m_RendererID);
		}

		void Bind() const override {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		}

		void Unbind() const override {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		const BufferLayout& GetLayout() const override {
			return m_Layout;
		}

		void SetLayout(const BufferLayout& layout) override {
			m_Layout = layout;/*

			auto& elements = layout.GetElements();
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, elements.size(), GL_FLOAT, GL_FALSE, element.Size, ())*/
		}
	};
} // EEngine
