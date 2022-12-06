#pragma once

#include <Renderer/IVertexArray.hpp>

namespace EEngine {
	class OpenGLVertexArray : public IVertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		void SetIndexBuffer(const Ref<IIndexBuffer>& indexBuffer) override;
		void AddVertexBuffer(const Ref<IVertexBuffer>& vertexBuffer) override;

		const Ref<IIndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		const std::vector<Ref<IVertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
	private:
		uint32_t m_RendererID;
		Ref<IIndexBuffer> m_IndexBuffer;
		std::vector<Ref<IVertexBuffer>> m_VertexBuffers;
	};
} // EEngine
