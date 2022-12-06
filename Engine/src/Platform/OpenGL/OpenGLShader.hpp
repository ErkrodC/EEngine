#pragma once

#include <glm/mat4x4.hpp>
#include <Renderer/IShader.hpp>

namespace EEngine {
	class OpenGLShader : public IShader {
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
	private:
		uint32_t m_RendererID;

		static void IndentLog(std::vector<char>& log);
	};
} // EEngine
