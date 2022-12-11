module;
#include <string>
#include <unordered_map>

export module EEngine.Rendering:ShaderLibrary;
import :IShader;
import EEngine.Core;

export namespace EEngine {
	class ShaderLibrary {
	public:
		void Add(const Ref<IShader>& shader);
		void Add(const std::string& name, const Ref<IShader>& shader);
		Ref<IShader> Load(const std::string& path);
		Ref<IShader> Load(const std::string& name, const std::string& path);

		bool TryGet(const std::string& name, Ref<IShader>* shader);
	private:
		std::unordered_map<std::string, Ref<IShader>> m_ShaderByName;
	};
} // EEngine
