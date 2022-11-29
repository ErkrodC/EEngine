#pragma once

namespace EEngine {
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		static VertexBuffer* Create(float* vertices, uint32_t size);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
} // EEngine
