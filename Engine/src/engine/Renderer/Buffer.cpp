#include <Platform/OpenGL/OpenGLBuffer.hpp>
#include "Buffer.hpp"
#include "Renderer.hpp"

namespace EEngine {
	Ref<IIndexBuffer> IIndexBuffer::Create(
		uint32_t* indices,
		uint32_t count
	) {
		switch (Renderer::GetSelectedAPI()) {
			case Renderer::API::None:
				EE_CORE_ERROR("No rendering API selected.");
				break;
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return nullptr;
	}

	Ref<IVertexBuffer> IVertexBuffer::Create(
		float* vertices,
		uint32_t size
	) {
		switch (Renderer::GetSelectedAPI()) {
			case Renderer::API::None:
				EE_CORE_ERROR("No rendering API selected.");
				break;
			case Renderer::API::OpenGL:
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return nullptr;
	}
} // EEngine