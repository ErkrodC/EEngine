#include <Platform/OpenGL/OpenGLVertexArray.hpp>
#include "IVertexArray.hpp"
#include "Renderer.hpp"

namespace EEngine {
	Ref<IVertexArray> IVertexArray::Create() {
		switch (Renderer::GetSelectedAPI()) {
			case Renderer::API::None: {
				EE_CORE_ERROR("No rendering API selected.");
				return nullptr;
			}
			case Renderer::API::OpenGL: {
				return std::make_shared<OpenGLVertexArray>();
			}
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return nullptr;
	}
} // EEngine