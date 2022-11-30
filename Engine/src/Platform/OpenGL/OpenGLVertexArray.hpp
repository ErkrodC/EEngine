#pragma once

#include <Renderer/IVertexArray.hpp>

namespace EEngine {
	class OpenGLVertexArray : public IVertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) override;
		void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) override;

		const std::shared_ptr<IIndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
	private:
		uint32_t m_RendererID;
		std::shared_ptr<IIndexBuffer> m_IndexBuffer;
		std::vector<std::shared_ptr<IVertexBuffer>> m_VertexBuffers;
	};
} // EEngine
