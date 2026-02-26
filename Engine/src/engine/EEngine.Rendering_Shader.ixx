module;
#include <glad/glad.h>
#include <fstream>
#include <sstream>

export module EEngine.Rendering:Shader;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLShader Implementation
	// ============================================================================
	class OpenGLShader {
	public:
		OpenGLShader(
			const std::string& name,
			const std::string& vertexSource,
			const std::string& fragmentSource
		) : m_Name(name) {
			CompileShaders({
				{ GL_VERTEX_SHADER, vertexSource },
				{ GL_FRAGMENT_SHADER, fragmentSource }
			});
		}

		explicit OpenGLShader(const std::string& path) {
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

		~OpenGLShader() { glDeleteProgram(m_RendererID); }
		OpenGLShader(const OpenGLShader& other) = delete;
		OpenGLShader& operator=(const OpenGLShader& other) = delete;

		OpenGLShader(OpenGLShader&& other) noexcept
			: m_RendererID(std::exchange(other.m_RendererID, 0)),
			  m_Name(std::move(other.m_Name)) {}

		OpenGLShader& operator=(OpenGLShader&& other) noexcept {
			glDeleteProgram(m_RendererID);
			m_RendererID = 0;

			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_Name = std::move(other.m_Name);

			return *this;
		}

		void Bind() const {
			glUseProgram(m_RendererID);
		}

		void Unbind() const {
			glUseProgram(0);
		}

		void SetInt(const std::string& name, int32_t value) {
			GLint location = GetUniformLocation(name);
			glUniform1i(location, value);
		}

		void SetFloat(const std::string& name, float value) {
			GLint location = GetUniformLocation(name);
			glUniform1f(location, value);
		}

		void SetFloat2(const std::string& name, const Math::vec2& values) {
			GLint location = GetUniformLocation(name);
			glUniform2f(location, values.x, values.y);
		}

		void SetFloat3(const std::string& name, const Math::vec3& values) {
			GLint location = GetUniformLocation(name);
			glUniform3f(location, values.x, values.y, values.z);
		}

		void SetFloat4(const std::string& name, const Math::vec4& values) {
			GLint location = GetUniformLocation(name);
			glUniform4f(location, values.x, values.y, values.z, values.w);
		}

		void SetMat3(const std::string& name, const Math::mat3& matrix) {
			GLint location = GetUniformLocation(name);
			glUniformMatrix3fv(location, 1, GL_FALSE, Math::value_ptr(matrix));
		}

		void SetMat4(const std::string& name, const Math::mat4& matrix) {
			GLint location = GetUniformLocation(name);
			glUniformMatrix4fv(location, 1, GL_FALSE, Math::value_ptr(matrix));
		}

		const std::string& GetName() const { return m_Name; }
	private:
		uint32_t m_RendererID;
		std::string m_Name;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

		static void IndentLog(std::vector<char>& log) {
			log.insert(log.begin(), '\t');

			for (auto it = log.end(); it != log.begin();) {
				if (*--it == '\n') {
					it = log.insert(++it, '\t');
					--it;
				}
			}
		}

		static bool TryCompileShader(GLenum shaderType, const std::string& source, GLuint* compiledShaderID) {
			*compiledShaderID = 0;
			GLuint shader = glCreateShader(shaderType);

			const auto* source_c_str = (const GLchar*) source.c_str();
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

				OpenGLShader::IndentLog(infoLog);
				Log::CoreError("Shader compilation failure:\n{0}", infoLog.data());
				return false;
			}

			*compiledShaderID = shader;
			return true;
		}

		std::string ReadFile(const std::string& path) {
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

		static GLenum ShaderTypeFromString(const std::string& type) {
			if (type == "vertex") {
				return GL_VERTEX_SHADER;
			} else if (type == "pixel" || type == "fragment") {
				return GL_FRAGMENT_SHADER;
			} else {
				Log::CoreError("Unknown shader type: {0}", type);
				return GL_NONE;
			}
		}

		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source) {
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

		void CompileShaders(const std::unordered_map<GLenum, std::string>& shaderSourceByType) {
			static const size_t MAX_SHADER_SOURCES = 2;

			size_t numShaderSources = shaderSourceByType.size();
			Log::CoreAssert(
				numShaderSources <= MAX_SHADER_SOURCES,
				"Number of shader sources exceeded max supported: {0} > {1}",
				numShaderSources,
				MAX_SHADER_SOURCES
			);

			std::array<GLuint, MAX_SHADER_SOURCES> compiledShaderIDs{};

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

			m_RendererID = glCreateProgram();

			for (auto& shaderID : compiledShaderIDs) {
				glAttachShader(m_RendererID, shaderID);
			}

			glLinkProgram(m_RendererID);

			GLint isLinked = 0;
			glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*) &isLinked);
			if (isLinked == GL_FALSE) {
				GLint maxLength = 0;
				glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

				glDeleteProgram(m_RendererID);
				for (auto& shaderID : compiledShaderIDs) {
					glDeleteShader(shaderID);
				}

				IndentLog(infoLog);
				Log::CoreError("Shader link failure:\n{0}", infoLog.data());
				return;
			}

			for (auto& shaderID : compiledShaderIDs) {
				glDetachShader(m_RendererID, shaderID);
			}
		}

		GLint GetUniformLocation(const std::string& name) {
			auto it = m_UniformLocationCache.find(name);
			if (it != m_UniformLocationCache.end()) {
				return it->second;
			}

			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			m_UniformLocationCache[name] = location;
			return location;
		}
	};

	// ============================================================================
	// Shader Type Alias
	// ============================================================================
	class DirectXShader; 
	class VulkanShader; 
	using Shader = std::conditional_t<g_API == API::DirectX, 
		DirectXShader, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLShader, 
			VulkanShader 
		> 
	>;

	// ============================================================================
	// Shader Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept ShaderConcept = requires(T shader, std::string name, int32_t i, float f, Math::vec2 v2, Math::vec3 v3, Math::vec4 v4, Math::mat3 m3, Math::mat4 m4) {
		{ shader.Bind() } -> std::same_as<void>;
		{ shader.Unbind() } -> std::same_as<void>;
		{ shader.SetInt(name, i) } -> std::same_as<void>;
		{ shader.SetFloat(name, f) } -> std::same_as<void>;
		{ shader.SetFloat2(name, v2) } -> std::same_as<void>;
		{ shader.SetFloat3(name, v3) } -> std::same_as<void>;
		{ shader.SetFloat4(name, v4) } -> std::same_as<void>;
		{ shader.SetMat3(name, m3) } -> std::same_as<void>;
		{ shader.SetMat4(name, m4) } -> std::same_as<void>;
		{ shader.GetName() } -> std::same_as<const std::string&>;
	};
	static_assert(ShaderConcept<Shader>);
}
