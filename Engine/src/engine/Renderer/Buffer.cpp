#include <Platform/OpenGL/OpenGLBuffer.hpp>
#include "Buffer.hpp"
#include "Renderer.hpp"

namespace EEngine {
	IndexBuffer* IndexBuffer::Create(
		uint32_t* indices,
		uint32_t count
	) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:
				EE_CORE_ERROR("No RendererAPI selected.");
				break;
			case RendererAPI::OpenGL:
				return new OpenGLIndexBuffer(indices, count);
		}

		EE_CORE_ERROR("Unknown RendererAPI.");
		return nullptr;
	}

	VertexBuffer* VertexBuffer::Create(
		float* vertices,
		uint32_t size
	) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:
				EE_CORE_ERROR("No RendererAPI selected.");
				break;
			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}

		EE_CORE_ERROR("Unknown RendererAPI.");
		return nullptr;
	}
} // EEngine