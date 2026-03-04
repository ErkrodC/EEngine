module;
#include <glad/glad.h>

module EEngine.Rendering;
import :Shader;

namespace EEngine::Rendering {
	// ============================================================================
	// Helpers
	// ============================================================================
	void IndentLog(std::vector<char>& log) {
		log.insert(log.begin(), '\t');

		for (auto it = log.end(); it != log.begin();) {
			if (*--it == '\n') {
				it = log.insert(++it, '\t');
				--it;
			}
		}
	}

	Expected<GLuint, std::string> TryCompileShader(GLuint shaderType, const std::string& source) {
		GLuint shader = glCreateShader(shaderType);

		const auto* source_c_str = source.c_str();
		glShaderSource(shader, 1, &source_c_str, nullptr);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			IndentLog(infoLog);
			return Unexpected("Shader compilation failure:\n{0}", infoLog.data());
		}

		return shader;
	}

	Expected<std::string, std::string> ReadFile(const std::string& path) {
		std::string result = "";
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (!in) { return Unexpected("Failed to open file {0}", path); }

		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();

		return result;
	}

	GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		} else if (type == "pixel" || type == "fragment") {
			return GL_FRAGMENT_SHADER;
		} else {
			Log::CoreError("Unknown shader type: {0}", type);
			return GL_NONE;
		}
	}

	Expected<std::unordered_map<GLenum, std::string>, std::string> Preprocess(const std::string& source) {
		std::unordered_map<GLenum, std::string> result;

		const char* typeToken = "#type";
		size_t typeTokenLength = std::strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			if (eol == std::string::npos) { return Unexpected("Syntax error in shader: missing newline after #type"); }
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			GLenum shaderType = ShaderTypeFromString(type);
			if (shaderType == GL_NONE) { return Unexpected("Invalid shader type specified: {0}", type); }

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			size_t shaderSourceLength = nextLinePos == std::string::npos
				? source.size() - 1
				: nextLinePos;

			result[shaderType] = source.substr(nextLinePos, pos - shaderSourceLength);
		}

		return result;
	}

	// ============================================================================
	// OpenGLShader Method Definitions
	// ============================================================================
	Expected<Shared<OpenGLShader>, std::string> OpenGLShader::Create(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) {
		return CompileShaders({
			{ GL_VERTEX_SHADER, vertexSource },
			{ GL_FRAGMENT_SHADER, fragmentSource }
		}).and_then([&](uint32_t rendererID) {
			return Expected<Shared<OpenGLShader>, std::string>(MakeShared<OpenGLShader>(name, rendererID));
		});
	}

	Expected<Shared<OpenGLShader>, std::string> OpenGLShader::Create(const std::string& path) {
		// extracts name from path
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos
			? 0
			: lastSlash + 1;

		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos
			? path.size() - lastSlash
			: lastDot - lastSlash;
		std::string name = path.substr(lastSlash, count);

		return ReadFile(path)
			.and_then(Preprocess)
			.and_then([](const auto& sources) { return CompileShaders(sources); })
			.and_then([&](uint32_t rendererID) {
				return Expected<Shared<OpenGLShader>, std::string>(MakeShared<OpenGLShader>(name, rendererID));
			});
	}

	OpenGLShader::OpenGLShader(const std::string& name, uint32_t rendererID)
		: m_RendererID(rendererID), m_Name(name) {}

	OpenGLShader::~OpenGLShader() { glDeleteProgram(m_RendererID); }

	OpenGLShader::OpenGLShader(OpenGLShader&& other) noexcept
		: m_RendererID(std::exchange(other.m_RendererID, 0)),
		  m_Name(std::move(other.m_Name)) {}

	OpenGLShader& OpenGLShader::operator=(OpenGLShader&& other) noexcept {
		glDeleteProgram(m_RendererID);
		// ER NOTE: `m_RendererID = 0` protects against double-delete due to self-assignment
		// i.e. enables std::exchange(other.m_RendererID, 0) to return 0, thus leaving this/other in a moved-from
		// state, and that calls glDeleteBuffers(1, &0) (no-op) when destructed.
		m_RendererID = 0;

		m_RendererID = std::exchange(other.m_RendererID, 0);
		m_Name = std::move(other.m_Name);

		return *this;
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int32_t value) const {
		GLint location = GetUniformLocation(name);
		glUniform1i(location, static_cast<GLint>(value));
	}

	void OpenGLShader::SetFloat(const std::string& name, float_t value) const {
		GLint location = GetUniformLocation(name);
		glUniform1f(location, static_cast<GLfloat>(value));
	}

	void OpenGLShader::SetFloat2(const std::string& name, const Math::vec2& values) const {
		GLint location = GetUniformLocation(name);
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const Math::vec3& values) const {
		GLint location = GetUniformLocation(name);
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const Math::vec4& values) const {
		GLint location = GetUniformLocation(name);
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const Math::mat3& matrix) const {
		GLint location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, Math::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string& name, const Math::mat4& matrix) const {
		GLint location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, Math::value_ptr(matrix));
	}

	Expected<uint32_t, std::string> OpenGLShader::CompileShaders(const std::unordered_map<uint32_t, std::string>& shaderSourceByType) {
		static const size_t MAX_SHADER_SOURCES = 2;

		size_t numShaderSources = shaderSourceByType.size();
		Log::CoreAssert(
			numShaderSources <= MAX_SHADER_SOURCES,
			"Number of shader sources exceeded max supported: {0} > {1}",
			numShaderSources,
			MAX_SHADER_SOURCES
		);

		std::array<GLuint, MAX_SHADER_SOURCES> compiledShaderIDs{};

		int32_t shaderIDIndex = 0;
		std::string errorMessage;
		for (auto& [shaderType, shaderSource] : shaderSourceByType) {
			auto compiled = TryCompileShader(shaderType, shaderSource);
			if (compiled) {
				compiledShaderIDs[shaderIDIndex++] = *compiled;
			} else {
				errorMessage = compiled.error();
				break;
			}
		}

		if (!errorMessage.empty()) {
			for (int i = 0; i < shaderIDIndex; ++i) {
				glDeleteShader(compiledShaderIDs[i]);
			}
			return Unexpected(errorMessage);
		}

		GLuint program = glCreateProgram();

		for (int i = 0; i < shaderIDIndex; ++i) {
			glAttachShader(program, compiledShaderIDs[i]);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*) &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (auto& shaderID : compiledShaderIDs) {
				glDeleteShader(shaderID);
			}

			IndentLog(infoLog);
			return Unexpected("Shader link failure:\n{0}", infoLog.data());
		}

		for (auto& shaderID : compiledShaderIDs) {
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}

		return program;
	}

	GLint OpenGLShader::GetUniformLocation(const std::string& name) const {
		auto it = m_UniformLocationCache.find(name);
		if (it != m_UniformLocationCache.end()) {
			return it->second;
		}

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocationCache[name] = location;
		return location;
	}
}
