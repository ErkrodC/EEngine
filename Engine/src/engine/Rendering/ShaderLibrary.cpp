module;
#include <memory>
#include <string>
#include "Core/Core.hpp"

module EEngine.Rendering:ShaderLibrary;
import :IShader;
import :RendererAPI;
import EEngine.Core;

namespace EEngine {
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
		auto shader = Renderer::CreateShader(path);
		Add(shader);
		return shader;
	}

	Ref<IShader> ShaderLibrary::Load(
		const std::string& name,
		const std::string& path
	) {
		auto shader = RendererAPI::CreateShader(path);
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