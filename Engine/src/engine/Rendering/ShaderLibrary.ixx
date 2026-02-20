export module EEngine.Rendering:ShaderLibrary;
import :IShader;
import :RendererAPI;
import EEngine.Core;
import EEngine.Standard;

export namespace EEngine {
	class ShaderLibrary {
	public:
		void Add(const Shared<IShader>& shader) {
			auto& name = shader->GetName();
			Add(name, shader);
		}

		void Add(const std::string& name, const Shared<IShader>& shader)  {
			Log::CoreAssert(m_ShaderByName.find(name) == m_ShaderByName.end(), "Tried to add duplicate shader.");
			m_ShaderByName[name] = shader;
		}

		Shared<IShader> Load(const std::string& path) {
			auto shader = RendererAPI::CreateShader(path);
			Add(shader);
			return shader;
		}

		Shared<IShader> Load(const std::string& name, const std::string& path) {
			auto shader = RendererAPI::CreateShader(path);
			Add(name, shader);
			return shader;
		}

		bool TryGet(const std::string& name, Shared<IShader>* shader) {
			bool foundShader = m_ShaderByName.find(name) != m_ShaderByName.end();
			Log::CoreAssert(foundShader, "Shader not found.");
			if (foundShader) {
				*shader = m_ShaderByName[name];
			}

			return foundShader;
		}
	private:
		std::unordered_map<std::string, Shared<IShader>> m_ShaderByName;
	};
} // EEngine
