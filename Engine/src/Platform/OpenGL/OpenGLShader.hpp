#pragma once

#include <glm/mat4x4.hpp>
#include <Renderer/IShader.hpp>

// ER TODO remove
typedef unsigned int GLenum;
typedef unsigned int GLuint;

namespace EEngine {
	class OpenGLShader : public IShader {
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		explicit OpenGLShader(const std::string& path);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		void UploadUniformInt(const std::string& name, int32_t value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;

		static void IndentLog(std::vector<char>& log);
		static bool TryCompileShader(
			GLenum shaderType,
			const std::string& source,
			GLuint* pInt
		);

		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void CompileShaders(const std::unordered_map<GLenum, std::string>& shaderSourceByType);
	};
} // EEngine
