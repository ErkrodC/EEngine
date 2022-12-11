module;
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <glad/glad.h>
#include "Core/Core.hpp"
DISABLE_WARNING_PUSH
DISABLE_WARNING_NAMELESS_STRUCT_UNION
#include <glm/gtc/type_ptr.hpp>
DISABLE_WARNING_POP

module EEngine.Rendering:OpenGLShader;

namespace EEngine {
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		} else if (type == "pixel" || type == "fragment") {
			return GL_FRAGMENT_SHADER;
		} else {
			EE_CORE_ERROR("Unknown shader type: {0}", type);
			return GL_NONE;
		}
	}

	OpenGLShader::OpenGLShader(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) : m_Name(name) {
		CompileShaders({
			{ GL_VERTEX_SHADER, vertexSource },
			{ GL_FRAGMENT_SHADER, fragmentSource }
		});
	}

	OpenGLShader::OpenGLShader(const std::string& path) {
		std::string shaderSource = ReadFile(path);
		auto shaderSourceByType = Preprocess(shaderSource);
		CompileShaders(shaderSourceByType);

		{
			// extracts name from path
			auto lastSlash = path.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos
				? 0
				: lastSlash + 1;

			auto lastDot = path.rfind('.');
			auto count = lastDot == std::string::npos
				? path.size() - lastSlash
				: lastDot - lastSlash;
			m_Name = path.substr(lastSlash, count);
		}
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
	};

	std::string OpenGLShader::ReadFile(const std::string& path) {
		// ER TODO consider virtual fs
		std::ifstream in(path, std::ios::in | std::ios::binary);
		std::string result;
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		} else {
			EE_CORE_ERROR("Failed to open file {0}", path);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& source) {
		std::unordered_map<GLenum, std::string> result;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			EE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			GLenum shaderType = ShaderTypeFromString(type);
			EE_CORE_ASSERT(shaderType, "Invalid shader type specified: {0}", type);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			size_t shaderSourceLength = nextLinePos == std::string::npos
				? source.size() - 1
				: nextLinePos;

			result[shaderType] = source.substr(nextLinePos, pos - shaderSourceLength);
		}

		return result;
	}

	void OpenGLShader::CompileShaders(const std::unordered_map<GLenum, std::string>& shaderSourceByType) {
		static const size_t MAX_SHADER_SOURCES = 2;

		size_t numShaderSources = shaderSourceByType.size();
		EE_CORE_ASSERT(
			numShaderSources <= MAX_SHADER_SOURCES,
			"Number of shader sources exceeded max supported: {0} > {1}",
			numShaderSources,
			MAX_SHADER_SOURCES
		);

		std::array<GLuint, MAX_SHADER_SOURCES> compiledShaderIDs{};
		//compiledShaderIDs.reserve(numShaderSources);

		int shaderIDIndex = 0;
		for (auto& pair : shaderSourceByType) {
			GLuint compiledShaderID = 0;
			if (TryCompileShader(pair.first, pair.second, &compiledShaderID)) {
				compiledShaderIDs[shaderIDIndex++] = compiledShaderID;
			} else {
				for (auto& shaderID : compiledShaderIDs) {
					glDeleteShader(shaderID);
				}
				return;
			}
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		for (auto& shaderID : compiledShaderIDs) {
			glAttachShader(m_RendererID, shaderID);
		}

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*) &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			for (auto& shaderID : compiledShaderIDs) {
				glDeleteShader(shaderID);
			}

			IndentLog(infoLog);
			EE_CORE_ERROR("Shader link failure:\n{0}", infoLog.data());
			return;
		}

		// Always detach shaders after a successful link.
		for (auto& shaderID : compiledShaderIDs) {
			glDetachShader(m_RendererID, shaderID);
		}
	}

	bool OpenGLShader::TryCompileShader(GLenum shaderType, const std::string& source, GLuint* compiledShaderID) {
		// Create an empty vertex shader handle
		*compiledShaderID = 0;
		GLuint shader = glCreateShader(shaderType);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const auto* source_c_str = (const GLchar*) source.c_str();
		glShaderSource(shader, 1, &source_c_str, nullptr);

		// Compile the vertex shader
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shader);

			OpenGLShader::IndentLog(infoLog);
			EE_CORE_ERROR("Shader compilation failure:\n{0}", infoLog.data());
			return false;
		}

		*compiledShaderID = shader;
		return true;
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(
		const std::string& name,
		int32_t value
	) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(
		const std::string& name,
		float value
	) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(
		const std::string& name,
		const glm::vec2& values
	) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(
		const std::string& name,
		const glm::vec3& values
	) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(
		const std::string& name,
		const glm::vec4& values
	) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(
		const std::string& name,
		const glm::mat3& matrix
	) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(
		const std::string& name,
		const glm::mat4& matrix
	) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::IndentLog(std::vector<char>& log) {
		log.insert(log.begin(), '\t');

		for (auto it = log.end(); it != log.begin();) {
			if (*--it == '\n') {
				it = log.insert(++it, '\t');
				--it;
			}
		}
	}
} // EEngine