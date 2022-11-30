#include <Platform/OpenGL/OpenGLVertexArray.hpp>
#include "VertexArray.hpp"
#include "Renderer.hpp"

namespace EEngine {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetSelectedAPI()) {
			case Renderer::API::None: {
				EE_CORE_ERROR("No rendering API selected.");
				return nullptr;
			}
			case Renderer::API::OpenGL: {
				return new OpenGLVertexArray();
			}
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return nullptr;
	}
} // EEngine