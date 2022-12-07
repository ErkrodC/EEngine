#include <Platform/OpenGL/OpenGLShader.hpp>
#include "IShader.hpp"
#include "Renderer.hpp"

namespace EEngine {
	Ref<IShader> IShader::Create(const std::string& vertexSource, const std::string& fragmentSource) {
		switch (Renderer::GetSelectedAPI()) {
			case Renderer::API::None: {
				EE_CORE_ERROR("No rendering API selected.");
				return nullptr;
			}
			case Renderer::API::OpenGL: {
				return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
			}
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return nullptr;
	}

	Ref<EEngine::IShader> IShader::Create(const std::string& path) {
		switch (Renderer::GetSelectedAPI()) {
			case Renderer::API::None: {
				EE_CORE_ERROR("No rendering API selected.");
				return nullptr;
			}
			case Renderer::API::OpenGL: {
				return std::make_shared<OpenGLShader>(path);
			}
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return nullptr;
	}
} // EEngine