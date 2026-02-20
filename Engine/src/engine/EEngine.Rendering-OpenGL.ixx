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

export namespace EEngine {
	// ============================================================================
	// OpenGL Buffer Implementations
	// ============================================================================

	class OpenGLIndexBuffer : public IIndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
			: m_Count(count) {
			glCreateBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		}

		~OpenGLIndexBuffer() override {
			glDeleteBuffers(1, &m_RendererID);
		}

		void Bind() const override {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		}

		void Unbind() const override {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLVertexBuffer : public IVertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size) {
			glCreateBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}

		~OpenGLVertexBuffer() override {
			glDeleteBuffers(1, &m_RendererID);
		}

		void Bind() const override {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		}

		void Unbind() const override {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		const BufferLayout& GetLayout() const override {
			return m_Layout;
		}

		void SetLayout(const BufferLayout& layout) override {
			m_Layout = layout;
		}
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout{};
	};

	// ============================================================================
	// OpenGL Vertex Array Implementation
	// ============================================================================

	class OpenGLVertexArray : public IVertexArray {
	public:
		OpenGLVertexArray() {
			glCreateVertexArrays(1, &m_RendererID);
		}

		~OpenGLVertexArray() override {
			glDeleteVertexArrays(1, &m_RendererID);
		}

		void Bind() const override {
			glBindVertexArray(m_RendererID);
		}

		void Unbind() const override {
			glBindVertexArray(0);
		}

		void SetIndexBuffer(const Shared<IIndexBuffer>& indexBuffer) override {
			glBindVertexArray(m_RendererID);
			indexBuffer->Bind();

			m_IndexBuffer = indexBuffer;
		}

		void AddVertexBuffer(const Shared<IVertexBuffer>& vertexBuffer) override {
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

		const Shared<IIndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		const std::vector<Shared<IVertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
	private:
		uint32_t m_RendererID;
		Shared<IIndexBuffer> m_IndexBuffer;
		std::vector<Shared<IVertexBuffer>> m_VertexBuffers;

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

	class OpenGLShader : public IShader {
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

		~OpenGLShader() override {
			glDeleteProgram(m_RendererID);
		}

		void Bind() const override {
			glUseProgram(m_RendererID);
		}

		void Unbind() const override {
			glUseProgram(0);
		}

		void SetInt(const std::string& name, int32_t value) override {
			UploadUniformInt(name, value);
		}

		void SetFloat(const std::string& name, float value) override {
			UploadUniformFloat(name, value);
		}

		void SetFloat2(const std::string& name, const Math::vec2& values) override {
			UploadUniformFloat2(name, values);
		}

		void SetFloat3(const std::string& name, const Math::vec3& values) override {
			UploadUniformFloat3(name, values);
		}

		void SetFloat4(const std::string& name, const Math::vec4& values) override {
			UploadUniformFloat4(name, values);
		}

		void SetMat3(const std::string& name, const Math::mat3& matrix) override {
			UploadUniformMat3(name, matrix);
		}

		void SetMat4(const std::string& name, const Math::mat4& matrix) override {
			UploadUniformMat4(name, matrix);
		}

		const std::string& GetName() const override { return m_Name; };
	private:
		uint32_t m_RendererID;
		std::string m_Name;

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
			size_t typeTokenLength = strlen(typeToken);
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

		void UploadUniformInt(const std::string& name, int32_t value) {
			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			glUniform1i(location, value);
		}

		void UploadUniformFloat(const std::string& name, float value) {
			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			glUniform1f(location, value);
		}

		void UploadUniformFloat2(const std::string& name, const Math::vec2& values) {
			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			glUniform2f(location, values.x, values.y);
		}

		void UploadUniformFloat3(const std::string& name, const Math::vec3& values) {
			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			glUniform3f(location, values.x, values.y, values.z);
		}

		void UploadUniformFloat4(const std::string& name, const Math::vec4& values) {
			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			glUniform4f(location, values.x, values.y, values.z, values.w);
		}

		void UploadUniformMat3(const std::string& name, const Math::mat3& matrix) {
			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			glUniformMatrix3fv(location, 1, GL_FALSE, Math::value_ptr(matrix));
		}

		void UploadUniformMat4(const std::string& name, const Math::mat4& matrix) {
			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			glUniformMatrix4fv(location, 1, GL_FALSE, Math::value_ptr(matrix));
		}
	};

	// ============================================================================
	// OpenGL Texture Implementation
	// ============================================================================

	class OpenGLTexture2D : public ITexture2D {
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

		~OpenGLTexture2D() override {
			glDeleteTextures(1, &m_RendererID);
		}

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void SetData(void* data, uint32_t size) override {
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

		void Bind() const override {
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

	class OpenGLContext : public IGraphicsContext {
	public:
		explicit OpenGLContext(GLFWwindow* window)
			: m_Window(window) {
			Log::Assert(m_Window, "Window was null when creating OpenGLContext");
		}

		void Initialize() override {
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

		void SwapBuffers() override {
			glfwSwapBuffers(m_Window);
		}
	private:
		GLFWwindow* m_Window;
	};

	// ============================================================================
	// OpenGL Renderer API
	// ============================================================================

	class OpenGLRendererAPI : public IRendererAPI {
	public:
		void InitializeImpl() override {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
			glViewport(x, y, width, height);
		}

		void ClearImpl(const Math::vec4& color) override {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void DrawIndexedImpl(const Shared<IVertexArray>& vertexArray) override {
			glDrawElements(GL_TRIANGLES,
				(GLsizei)vertexArray->GetIndexBuffer()->GetCount(),
				GL_UNSIGNED_INT,
				nullptr
			);
		}

		Shared<IIndexBuffer> CreateIndexBufferImpl(uint32_t* indices, uint32_t count) override {
			return MakeShared<OpenGLIndexBuffer>(indices, count);
		}

		Shared<IVertexBuffer> CreateVertexBufferImpl(float* vertices, uint32_t size) override {
			return MakeShared<OpenGLVertexBuffer>(vertices, size);
		}

		Shared<IShader> CreateShaderImpl(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) override {
			return MakeShared<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		Shared<IShader> CreateShaderImpl(const std::string& path) override {
			return MakeShared<OpenGLShader>(path);
		}

		Shared<IVertexArray> CreateVertexArrayImpl() override {
			return MakeShared<OpenGLVertexArray>();
		}

		Shared<ITexture2D> CreateTexture2DImpl(const std::string& path) override {
			return MakeShared<OpenGLTexture2D>(path);
		}

		Shared<ITexture2D> CreateTexture2DImpl(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) override {
			return MakeShared<OpenGLTexture2D>(width, height, data, size);
		}
	};
}
