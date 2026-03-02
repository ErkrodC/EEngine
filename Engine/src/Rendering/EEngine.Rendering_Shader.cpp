module;
#include <glad/glad.h>

module EEngine.Rendering;
import :Shader;

namespace EEngine::Rendering {
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

	void IndentLog(std::vector<char>& log) {
		log.insert(log.begin(), '\t');

		for (auto it = log.end(); it != log.begin();) {
			if (*--it == '\n') {
				it = log.insert(++it, '\t');
				--it;
			}
		}
	}

	bool TryCompileShader(GLuint shaderType, const std::string& source, GLuint& compiledShaderID) {
		compiledShaderID = 0;
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
			Log::CoreError("Shader compilation failure:\n{0}", infoLog.data());
			return false;
		}

		compiledShaderID = shader;
		return true;
	}

	std::string OpenGLShader::ReadFile(const std::string& path) {
		std::string result = "";
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		} else {
			Log::CoreError("Failed to open file {0}", path);
		}

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

	std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& source) {
		std::unordered_map<GLenum, std::string> result;

		const char* typeToken = "#type";
		size_t typeTokenLength = std::strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			Log::CoreAssert(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			GLenum shaderType = ShaderTypeFromString(type);
			Log::CoreAssert(shaderType, "Invalid shader type specified: {0}", type);

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
		Log::CoreAssert(
			numShaderSources <= MAX_SHADER_SOURCES,
			"Number of shader sources exceeded max supported: {0} > {1}",
			numShaderSources,
			MAX_SHADER_SOURCES
		);

		std::array<GLuint, MAX_SHADER_SOURCES> compiledShaderIDs{};

		int32_t shaderIDIndex = 0;
		bool success = true;
		std::ranges::for_each(shaderSourceByType, [&](auto& pair) {
			auto [shaderType, shaderSource] = pair;
			GLuint compiledShaderID = 0;
			if (TryCompileShader(shaderType, shaderSource, compiledShaderID)) {
				compiledShaderIDs[shaderIDIndex++] = compiledShaderID;
			} else {
				std::ranges::for_each(compiledShaderIDs, glDeleteShader);
				success = false;
			}
		});

		if (!success) { return; }

		m_RendererID = glCreateProgram();

		std::ranges::for_each(compiledShaderIDs, [&](auto& shaderID) {
			glAttachShader(m_RendererID, shaderID);
		});

		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*) &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_RendererID);
			std::ranges::for_each(compiledShaderIDs, glDeleteShader);

			IndentLog(infoLog);
			Log::CoreError("Shader link failure:\n{0}", infoLog.data());
			return;
		}

		std::ranges::for_each(compiledShaderIDs, [&](auto& shaderID) {
			glDetachShader(m_RendererID, shaderID);
		});
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
