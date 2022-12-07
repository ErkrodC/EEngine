#pragma once

namespace EEngine {
	class IShader {
	public:
		virtual ~IShader() = default;

		static Ref<EEngine::IShader> Create(const std::string& path);
		static Ref <EEngine::IShader> Create(
			const std::string& name,
			const std::string& vertexSource,
			const std::string& fragmentSource
		);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;
	};

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
