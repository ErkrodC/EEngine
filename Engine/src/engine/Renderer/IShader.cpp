#include <Platform/OpenGL/OpenGLShader.hpp>
#include "IShader.hpp"
#include "Renderer.hpp"

namespace EEngine {
	Ref <EEngine::IShader> IShader::Create(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) {
		switch (Renderer::GetSelectedAPI()) {
			case Renderer::API::None: {
				EE_CORE_ERROR("No rendering API selected.");
				return nullptr;
			}
			case Renderer::API::OpenGL: {
				return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
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

	void ShaderLibrary::Add(const Ref<IShader>& shader) {
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(
		const std::string& name,
		const Ref<IShader>& shader
	) {
		EE_CORE_ASSERT(m_ShaderByName.find(name) == m_ShaderByName.end(), "Tried to add duplicate shader.");
		m_ShaderByName[name] = shader;
	}

	Ref <IShader> ShaderLibrary::Load(const std::string& path) {
		auto shader = IShader::Create(path);
		Add(shader);
		return shader;
	}

	Ref<IShader> ShaderLibrary::Load(
		const std::string& name,
		const std::string& path
	) {
		auto shader = IShader::Create(path);
		Add(name, shader);
		return shader;
	}

	bool ShaderLibrary::TryGet(
		const std::string& name,
		Ref<IShader>* shader
	) {
		bool foundShader = m_ShaderByName.find(name) != m_ShaderByName.end();
		EE_CORE_ASSERT(foundShader, "Shader not found.");
		if (foundShader) {
			*shader = m_ShaderByName[name];
		}

		return foundShader;
	}
} // EEngine