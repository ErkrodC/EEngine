#pragma once

#include <Renderer/VertexArray.hpp>

namespace EEngine {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;

		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
	private:
		uint32_t m_RendererID;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	};
} // EEngine
