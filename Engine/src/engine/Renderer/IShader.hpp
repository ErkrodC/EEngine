#pragma once

namespace EEngine {
	class IShader {
	public:
		virtual ~IShader() = default;

		static Ref<EEngine::IShader> Create(const std::string& path);
		static Ref<IShader> Create(const std::string& vertexSource, const std::string& fragmentSource);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
} // EEngine
