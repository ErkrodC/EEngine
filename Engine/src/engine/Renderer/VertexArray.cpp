#include <Platform/OpenGL/OpenGLVertexArray.hpp>
#include "VertexArray.hpp"
#include "Renderer.hpp"

namespace EEngine {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None: {
				EE_CORE_ERROR("No RendererAPI selected.");
				return nullptr;
			}
			case RendererAPI::OpenGL: {
				return new OpenGLVertexArray();
			}
		}

		EE_CORE_ERROR("Unknown RendererAPI.");
		return nullptr;
	}
} // EEngine