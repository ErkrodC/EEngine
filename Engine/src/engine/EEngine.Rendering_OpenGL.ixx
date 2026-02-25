module;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.hpp>

export module EEngine.Rendering:OpenGL;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :Buffers;
import :Interfaces;

using namespace EEngine;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGL Buffer Implementations
	// ============================================================================
	class OpenGLIndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
			: m_Count(count)
		{
			glCreateBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		}

		~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }
		OpenGLIndexBuffer(const OpenGLIndexBuffer& other) = delete;
		OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer& other) = delete;

		OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept
			: m_RendererID(std::exchange(other.m_RendererID, 0)),
			  m_Count(std::move(other.m_Count)) { }

		OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& other) noexcept {
			glDeleteBuffers(1, &m_RendererID);
			// ER NOTE: `m_RendererID = 0` protects against double-delete due to self-assignment
			// i.e. enables std::exchange(other.m_RendererID, 0) to return 0, thus leaving this/other in a moved-from
			// state, and that calls glDeleteBuffers(1, &0) (no-op) when destructed.
			m_RendererID = 0;

			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_Count = std::move(other.m_Count);

			return *this;
		}

		void Bind() const  {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		}

		void Unbind() const {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLVertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size) {
			glCreateBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }
		OpenGLVertexBuffer(const OpenGLVertexBuffer& other) = delete;
		OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer& other) = delete;

		OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept
			: m_RendererID(std::exchange(other.m_RendererID, 0)),
			m_Layout(std::move(other.m_Layout)) {}

		OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& other) noexcept {
			glDeleteBuffers(1, &m_RendererID);
			m_RendererID = 0;

			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_Layout = std::move(other.m_Layout);

			return *this;
		}

		void Bind() const  {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		}

		void Unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		const BufferLayout& GetLayout() const  {
			return m_Layout;
		}

		void SetLayout(const BufferLayout& layout) {
			m_Layout = layout;
		}
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout{};
	};

	// ============================================================================
	// OpenGL Vertex Array Implementation
	// ============================================================================
	class OpenGLVertexArray {
	public:
		OpenGLVertexArray() { glCreateVertexArrays(1, &m_RendererID); }
		~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }
		OpenGLVertexArray(const OpenGLVertexArray& other) = delete;
		OpenGLVertexArray& operator=(const OpenGLVertexArray& other) = delete;

		OpenGLVertexArray(OpenGLVertexArray&& other) noexcept
			: m_RendererID(std::exchange(other.m_RendererID, 0)),
			  m_IndexBuffer(std::move(other.m_IndexBuffer)),
			  m_VertexBuffers(std::move(other.m_VertexBuffers)) {}

		OpenGLVertexArray& operator=(OpenGLVertexArray&& other) noexcept {
			glDeleteVertexArrays(1, &m_RendererID);
			m_RendererID = 0;

			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_IndexBuffer = std::move(other.m_IndexBuffer);
			m_VertexBuffers = std::move(other.m_VertexBuffers);
			return *this;
		}

		void Bind() const {
			glBindVertexArray(m_RendererID);
		}

		void Unbind() const {
			glBindVertexArray(0);
		}

		void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer) {
			glBindVertexArray(m_RendererID);
			indexBuffer->Bind();

			m_IndexBuffer = indexBuffer;
		}

		void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) {
			Log::CoreAssert(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout.");

			glBindVertexArray(m_RendererID);
			vertexBuffer->Bind();

			uint32_t index = 0;
			const auto& layout = vertexBuffer->GetLayout();
			for (const auto& element : layout) {
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(
					index++,
					(GLint)element.ComponentCount,
					ShaderDataToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					(GLsizei)layout.GetStride(),
					(const void *)(intptr_t)element.Offset
				);
			}

			m_VertexBuffers.push_back(vertexBuffer);
		}

		const Shared<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
		const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
	private:
		uint32_t m_RendererID;
		Shared<IndexBuffer> m_IndexBuffer;
		std::vector<Shared<VertexBuffer>> m_VertexBuffers;

		static GLenum ShaderDataToOpenGLBaseType(ShaderData type) {
			switch (type) {
				case ShaderData::None:		break;

				case ShaderData::Float:
				case ShaderData::Float2:
				case ShaderData::Float3:
				case ShaderData::Float4:
				case ShaderData::Mat3:
				case ShaderData::Mat4:		return GL_FLOAT;

				case ShaderData::Int:
				case ShaderData::Int2:
				case ShaderData::Int3:
				case ShaderData::Int4:			return GL_INT;

				case ShaderData::Bool:			return GL_BOOL;
			}

			Log::CoreError("Unknown shader data type.");
			return GL_NONE;
		}
	};

	// ============================================================================
	// OpenGL Shader Implementation
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
	// OpenGL Texture Implementation
	// ============================================================================
	class OpenGLTexture2D {
	public:
		explicit OpenGLTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0)
			: m_Width(width)
			, m_Height(height)
		{
			GLenum storageFormat = GL_RGBA8;
			m_InstanceFormat = GL_RGBA;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, storageFormat, width, height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (!data) {
				m_Width = 1;
				m_Height = 1;
				uint8_t fallback[4] = { 255, 255, 255, 255 };
				data = fallback;
				size = sizeof(fallback);
			}

			SetData(data, size);
		}

		explicit OpenGLTexture2D(const std::string& path) : m_Path(path) {
			int width, height, numChannels;

			stbi_set_flip_vertically_on_load(true);
			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
			Log::CoreAssert(data, "Failed to load image.");
			m_Width = width;
			m_Height = height;

			GLenum storageFormat = GL_NONE;
			m_InstanceFormat = GL_NONE;
			switch (numChannels) {
				case 4: {
					storageFormat = GL_RGBA8;
					m_InstanceFormat = GL_RGBA;
					break;
				}
				case 3: {
					storageFormat = GL_RGB8;
					m_InstanceFormat = GL_RGB;
					break;
				}
			}

			Log::CoreAssert(storageFormat & m_InstanceFormat, "Format not supported.");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, storageFormat, width, height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			SetData(data, m_Width * m_Height * GetBytesPerPixel());

			stbi_image_free(data);
		}

		~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D& other) = delete;

		OpenGLTexture2D(OpenGLTexture2D&& other) noexcept
			: m_Path(std::move(other.m_Path)),
			  m_Width(std::move(other.m_Width)),
			  m_Height(std::move(other.m_Height)),
			  m_RendererID(std::exchange(other.m_RendererID, 0)),
			  m_InstanceFormat(std::move(other.m_InstanceFormat)) {}

		OpenGLTexture2D& operator=(OpenGLTexture2D&& other) noexcept {
			glDeleteTextures(1, &m_RendererID);
			m_RendererID = 0;

			m_Path = std::move(other.m_Path);
			m_Width = std::move(other.m_Width);
			m_Height = std::move(other.m_Height);
			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_InstanceFormat = std::move(other.m_InstanceFormat);

			return *this;
		}

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		void SetData(void* data, uint32_t size) {
			uint32_t expectedSize = m_Width * m_Height * GetBytesPerPixel();
			Log::CoreAssert(
				size == expectedSize,
				"Incorrect size when setting texture data. Expected {}, got {}",
				expectedSize,
				size
			);

			glTextureSubImage2D(
				m_RendererID,
				0,
				0,
				0,
				m_Width,
				m_Height,
				m_InstanceFormat,
				GL_UNSIGNED_BYTE,
				data
			);
		}

		void Bind() const {
			glBindTextureUnit(0, m_RendererID);
		}
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InstanceFormat;

		uint32_t GetBytesPerPixel() {
			switch (m_InstanceFormat) {
				case GL_RGBA: return 4;
				case GL_RGB: return 3;

				default:
					Log::CoreError("Unsupported texture instance format.");
					return 0;
			}
		}
	};

	// ============================================================================
	// OpenGL Graphics Context
	// ============================================================================
	class OpenGLContext {
	public:
		explicit OpenGLContext(GLFWwindow* window)
			: m_Window(window)
		{
			Log::Assert(m_Window, "Window was null when creating OpenGLContext");

			glfwMakeContextCurrent(m_Window);
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			Log::CoreAssert(status, "Failed to initialize GLAD!");

			Log::CoreInfo(
				"OpenGL Info:\n"
				"\tVendor: {0}\n"
				"\tRenderer: {1}\n"
				"\tVersion: {2}",
				(char*)glGetString(GL_VENDOR),
				(char*)glGetString(GL_RENDERER),
				(char*)glGetString(GL_VERSION)
			);
		}

		void SwapBuffers() const {
			glfwSwapBuffers(m_Window);
		}
	private:
		// non-owning, window guaranteed to outlive context because it is declared before
		// (thus destroyed after) the IGraphicsContext member of class WindowsWindow
		GLFWwindow* m_Window;
	};

	// ============================================================================
	// OpenGL Renderer API
	// ============================================================================
	class OpenGLRendererAPI {
	public:
		void Initialize() {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			glViewport(x, y, width, height);
		}

		void Clear(const Math::vec4& color) {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void DrawIndexed(const Shared<VertexArray>& vertexArray) {
			glDrawElements(GL_TRIANGLES,
				(GLsizei)vertexArray->GetIndexBuffer()->GetCount(),
				GL_UNSIGNED_INT,
				nullptr
			);
		}

		Shared<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) {
			return MakeShared<OpenGLIndexBuffer>(indices, count);
		}

		Shared<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) {
			return MakeShared<OpenGLVertexBuffer>(vertices, size);
		}

		Shared<Shader> CreateShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) {
			return MakeShared<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		Shared<Shader> CreateShader(const std::string& path) {
			return MakeShared<OpenGLShader>(path);
		}

		Shared<VertexArray> CreateVertexArray() {
			return MakeShared<OpenGLVertexArray>();
		}

		Shared<Texture2D> CreateTexture2D(const std::string& path) {
			return MakeShared<OpenGLTexture2D>(path);
		}

		Shared<Texture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) {
			return MakeShared<OpenGLTexture2D>(width, height, data, size);
		}

		bool TryGetOrLoadShader(const std::string& name, Shared<Shader>& outShader) {
			outShader = m_ShaderByName.contains(name)
				? m_ShaderByName[name]
				: CreateAndCacheShader(name);
			return outShader != nullptr;
		}

	private:
		std::unordered_map<std::string, Shared<Shader>> m_ShaderByName;

		Shared<Shader> CreateAndCacheShader(const std::string& path) {
			Shared<Shader> shader = CreateShader(path);
			auto& name = shader->GetName();
			Log::CoreAssert(!m_ShaderByName.contains(name), "Tried to add duplicate shader.");
			m_ShaderByName[name] = shader;
			return shader;
		}
	};
}
