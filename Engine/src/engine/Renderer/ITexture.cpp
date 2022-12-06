#include <Platform/OpenGL/OpenGLTexture.hpp>
#include "ITexture.hpp"
#include "Renderer.hpp"

namespace EEngine {
	Ref<ITexture2D> ITexture2D::Create(const std::string& path) {
		switch (Renderer::GetSelectedAPI()) {
			case Renderer::API::None: {
				EE_CORE_ERROR("No rendering API selected.");
				return nullptr;
			}
			case Renderer::API::OpenGL: {
				return std::make_shared<OpenGLTexture2D>(path);
			}
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return nullptr;
	}
} // EEngine