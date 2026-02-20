module;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.hpp>
// Workaround: GLM operators don't get found via ADL when re-exported with 'using' declarations.
// Need direct include in global module fragment for operator* to work on glm::mat4.
#include <glm/detail/type_mat4x4.hpp>

export module EEngine.Rendering;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Rendering API Enum
	// ============================================================================

	namespace Renderer {
		enum class API {
			None = 0,
			OpenGL = 1,
		};
	}

	// ============================================================================
	// Buffer & Layout (API-agnostic interfaces)
	// ============================================================================

	enum class ShaderData {
		None,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	inline uint32_t ShaderDataSize(ShaderData type) {
		switch (type) {
			case ShaderData::None:		break;
			case ShaderData::Float:		return 4;
			case ShaderData::Float2:		return 4 * 2;
			case ShaderData::Float3:		return 4 * 3;
			case ShaderData::Float4:		return 4 * 4;
			case ShaderData::Mat3:		return 4 * 3 * 3;
			case ShaderData::Mat4:		return 4 * 4 * 4;
			case ShaderData::Int:			return 4;
			case ShaderData::Int2:		return 4 * 2;
			case ShaderData::Int3:		return 4 * 3;
			case ShaderData::Int4:		return 4 * 4;
			case ShaderData::Bool:		return 1;
		}

		Log::CoreError("Unknown shader data type.");
		return 0;
	}

	inline uint32_t ShaderDataComponentCount(ShaderData type) {
		switch (type) {
			case ShaderData::None:		break;
			case ShaderData::Float:		return 1;
			case ShaderData::Float2:		return 2;
			case ShaderData::Float3:		return 3;
			case ShaderData::Float4:		return 4;
			case ShaderData::Mat3:		return 3 * 3;
			case ShaderData::Mat4:		return 4 * 4;
			case ShaderData::Int:			return 1;
			case ShaderData::Int2:		return 2;
			case ShaderData::Int3:		return 3;
			case ShaderData::Int4:		return 4;
			case ShaderData::Bool:		return 1;
		}

		Log::CoreError("Unknown shader data type.");
		return 0;
	}

	struct BufferElement {
		std::string Name;
		ShaderData Type;
		uint32_t Offset;
		uint32_t Size;
		uint32_t ComponentCount;
		bool Normalized;

		BufferElement(
			ShaderData type,
			const std::string& name,
			bool normalized = false
		) : Name(name),
			Type(type),
			Size(ShaderDataSize(type)),
			Offset(0),
			ComponentCount(ShaderDataComponentCount(type)),
			Normalized(normalized) {}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements) {
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;

		void CalculateOffsetsAndStride() {
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}
	};

	class IIndexBuffer {
	public:
		virtual ~IIndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;
	};

	class IVertexBuffer {
	public:
		virtual ~IVertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
	};

	// ============================================================================
	// Shader (API-agnostic interface)
	// ============================================================================

	class IShader {
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int32_t value) = 0;

		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const Math::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const Math::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const Math::vec4& value) = 0;

		virtual void SetMat3(const std::string& name, const Math::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const Math::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;
	};

	// ============================================================================
	// Texture (API-agnostic interface)
	// ============================================================================

	class ITexture {
	public:
		virtual ~ITexture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind() const = 0;
	};

	class ITexture2D : public ITexture { };

	// ============================================================================
	// Vertex Array (API-agnostic interface)
	// ============================================================================

	class IVertexArray {
	public:
		virtual ~IVertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetIndexBuffer(const Shared<IIndexBuffer>& indexBuffer) = 0;
		virtual void AddVertexBuffer(const Shared<IVertexBuffer>& vertexBuffer) = 0;

		virtual const Shared<IIndexBuffer>& GetIndexBuffer() const = 0;
		virtual const std::vector<Shared<IVertexBuffer>>& GetVertexBuffers() const = 0;
	};

	// ============================================================================
	// Graphics Context (API-agnostic interface)
	// ============================================================================

	class IGraphicsContext {
	public:
		virtual void Initialize() = 0;
		virtual void SwapBuffers() = 0;
	};

	// ============================================================================
	// Renderer API (API-agnostic interface)
	// ============================================================================

	class IRendererAPI {
	public:
		virtual void InitializeImpl() = 0;
		virtual void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void ClearImpl(const Math::vec4& color) = 0;
		virtual void DrawIndexedImpl(const Shared<IVertexArray>& vertexArray) = 0;
		virtual Shared<IIndexBuffer> CreateIndexBufferImpl(uint32_t* indices, uint32_t count) = 0;
		virtual Shared<IVertexBuffer> CreateVertexBufferImpl(float* vertices, uint32_t size) = 0;
		virtual Shared<IShader> CreateShaderImpl(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) = 0;
		virtual Shared<IShader> CreateShaderImpl(const std::string& path) = 0;
		virtual Shared<IVertexArray> CreateVertexArrayImpl() = 0;
		virtual Shared<ITexture2D> CreateTexture2DImpl(const std::string& path) = 0;
		virtual Shared<ITexture2D> CreateTexture2DImpl(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) = 0;
	};

	// ============================================================================
	// Camera
	// ============================================================================

	class Camera {
	public:
		explicit Camera(const Math::mat4& projection)
			: m_Projection(projection)
			, m_Position(Math::vec3(0.0f, 0.0f, 0.0f))
			, m_Rotation(Math::quat(1.0f, 0.0f, 0.0f, 0.0f)
		) {
			RecalculateViewMatrix();
		}

		void SetProjection(const Math::mat4& projection) {
			m_Projection = projection;
			m_ProjectionView = m_Projection * m_View;
		}

		const Math::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const Math::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const Math::quat& GetRotation() const { return m_Rotation; }
		void SetRotation(const Math::quat& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const Math::mat4& GetProjectionMatrix() const { return m_Projection; }
		const Math::mat4& GetViewMatrix() const { return m_View; }
		const Math::mat4& GetProjectionViewMatrix() const { return m_ProjectionView; }
	private:
		Math::mat4 m_Projection;
		Math::mat4 m_View;
		Math::mat4 m_ProjectionView;

		Math::vec3 m_Position;
		Math::quat m_Rotation;

		void RecalculateViewMatrix() {
			Math::mat4 transform = Math::translate(Math::mat4(1.0f), m_Position) * Math::toMat4(m_Rotation);
			m_View = Math::inverse(transform);
			m_ProjectionView = m_Projection * m_View;
		}
	};
} // EEngine

// ============================================================================
// OpenGL Implementations
// ============================================================================

export namespace EEngine {
	// OpenGL Buffer Implementations
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

	// OpenGL Vertex Array Implementation
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

	// OpenGL Shader Implementation
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

	// OpenGL Texture Implementation
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

	// OpenGL Graphics Context
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

	// OpenGL Renderer API
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
} // EEngine

// ============================================================================
// Renderer API Facade & Shader Library
// ============================================================================

export namespace EEngine {
	class ShaderLibrary {
	public:
		void Add(const Shared<IShader>& shader) {
			auto& name = shader->GetName();
			Add(name, shader);
		}

		void Add(const std::string& name, const Shared<IShader>& shader)  {
			Log::CoreAssert(m_ShaderByName.find(name) == m_ShaderByName.end(), "Tried to add duplicate shader.");
			m_ShaderByName[name] = shader;
		}

		Shared<IShader> Load(const std::string& path);

		Shared<IShader> Load(const std::string& name, const std::string& path);

		bool TryGet(const std::string& name, Shared<IShader>* shader) {
			bool foundShader = m_ShaderByName.find(name) != m_ShaderByName.end();
			Log::CoreAssert(foundShader, "Shader not found.");
			if (foundShader) {
				*shader = m_ShaderByName[name];
			}

			return foundShader;
		}
	private:
		std::unordered_map<std::string, Shared<IShader>> m_ShaderByName;
	};
}

namespace EEngine::RendererAPI {
	inline IRendererAPI& GetRendererAPI() {
		static OpenGLRendererAPI instance;
		return instance;
	}

	export inline void Initialize() {
		GetRendererAPI().InitializeImpl();
	}

	export inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		GetRendererAPI().SetViewportImpl(x, y, width, height);
	}

	export inline void Clear(const Math::vec4& color) {
		GetRendererAPI().ClearImpl(color);
	}

	export inline void DrawIndexed(const Shared<IVertexArray>& vertexArray) {
		GetRendererAPI().DrawIndexedImpl(vertexArray);
	}

	export inline Shared<IIndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) {
		return GetRendererAPI().CreateIndexBufferImpl(indices, count);
	}

	export inline Shared<IVertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) {
		return GetRendererAPI().CreateVertexBufferImpl(vertices, size);
	}

	export inline Shared<IShader> CreateShader(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) {
		return GetRendererAPI().CreateShaderImpl(name, vertexSource, fragmentSource);
	}

	export inline Shared<IShader> CreateShader(const std::string& path) {
		return GetRendererAPI().CreateShaderImpl(path);
	}

	export inline Shared<IVertexArray> CreateVertexArray() {
		return GetRendererAPI().CreateVertexArrayImpl();
	}

	export inline Shared<ITexture2D> CreateTexture2D(const std::string& path) {
		return GetRendererAPI().CreateTexture2DImpl(path);
	}

	export inline Shared<ITexture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) {
		return GetRendererAPI().CreateTexture2DImpl(width, height, data, size);
	}
} // EEngine::RendererAPI

namespace EEngine {
	Shared<IShader> ShaderLibrary::Load(const std::string& path) {
		auto shader = RendererAPI::CreateShader(path);
		Add(shader);
		return shader;
	}

	Shared<IShader> ShaderLibrary::Load(const std::string& name, const std::string& path) {
		auto shader = RendererAPI::CreateShader(path);
		Add(name, shader);
		return shader;
	}
}

// ============================================================================
// High-Level Renderer
// ============================================================================

namespace EEngine::Renderer {
	struct SceneData {
		Math::mat4 ProjectionView;
	};

	inline SceneData& GetSceneData() {
		static SceneData instance;
		return instance;
	}

	inline Shared<ShaderLibrary>& GetShaderLibraryInstance() {
		static Shared<ShaderLibrary> instance = MakeShared<ShaderLibrary>();
		return instance;
	}

	static API s_SelectedAPI = API::OpenGL;

	export void Initialize() {
		RendererAPI::Initialize();
	}

	export void OnWindowResized(uint32_t width, uint32_t height) {
		RendererAPI::SetViewport(0, 0, width, height);
	}

	export void BeginScene(const Camera& camera) {
		GetSceneData().ProjectionView = camera.GetProjectionViewMatrix();
	}

	export void EndScene() {

	}

	export void Submit(
		const Shared<IShader>& shader,
		const Shared<IVertexArray>& vertexArray,
		const Math::mat4& transform = Math::mat4(1.0f)
	) {
		shader->Bind();
		shader->SetMat4("u_ProjectionView", GetSceneData().ProjectionView);
		shader->SetMat4("u_Transform", transform);
		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}

	export inline API GetSelectedAPI() { return s_SelectedAPI; }
	export std::string GetRendererAPIString(API api) {
		switch (api) {
			case API::None:
				return "None";
			case API::OpenGL:
				return "OpenGL";
		}

		Log::CoreError("Unknown rendering API.");
		return "";
	}

	export inline Shared<ShaderLibrary> GetShaderLibrary() { return GetShaderLibraryInstance(); }
} // EEngine::Renderer

// ============================================================================
// 2D Renderer
// ============================================================================

namespace EEngine::Renderer2D {
	struct Renderer2DData {
		Shared<IVertexArray> QuadVertexArray{};
		Shared<IShader> TextureShader{};
		Shared<ITexture2D> WhiteTexture{};
	} *s_Data ;

	export void Initialize() {
		s_Data = new Renderer2DData();
		s_Data->QuadVertexArray = RendererAPI::CreateVertexArray();
		s_Data->WhiteTexture = RendererAPI::CreateTexture2D(1, 1);

		float vertices[4*5] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,
		};

		Shared<IVertexBuffer> vertexBuffer = RendererAPI::CreateVertexBuffer(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ ShaderData::Float3, "a_Position" },
			{ ShaderData::Float2, "v_TexCoord" }
		});
		s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Shared<IIndexBuffer> indexBuffer = RendererAPI::CreateIndexBuffer(indices, sizeof(indices)/sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_Data->TextureShader = Renderer::GetShaderLibrary()->Load("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	export void Shutdown() {
		delete s_Data;
	}

	export void BeginScene(const Camera& camera) {
		auto textureShader = s_Data->TextureShader;
		textureShader->Bind();
		textureShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
	}

	export void EndScene() {

	}

	export void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) {
		auto shader = s_Data->TextureShader;
		shader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		Math::mat4 pos = Math::translate(Math::Identity::mat4, position);
		Math::mat4 rot = Math::Identity::mat4; // ER TODO rotation
		Math::mat4 scale = Math::scale(Math::Identity::mat4, { size.x, size.y, 1.0f });
		Math::mat4 transform = pos * rot * scale;
		shader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererAPI::DrawIndexed(s_Data->QuadVertexArray);
	}

	export void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) {
		DrawQuad({position.x, position.y, 0.0f }, size, color);
	}

	export void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<ITexture2D>& texture) {
		auto shader = s_Data->TextureShader;
		shader->SetFloat4("u_Color", Math::vec4(1.0f));
		texture->Bind();

		Math::mat4 pos = Math::translate(Math::Identity::mat4, position);
		Math::mat4 rot = Math::Identity::mat4; // ER TODO rotation
		Math::mat4 scale = Math::scale(Math::Identity::mat4, { size.x, size.y, 1.0f });
		Math::mat4 transform = pos * rot * scale;
		shader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererAPI::DrawIndexed(s_Data->QuadVertexArray);
	}

	export void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Shared<ITexture2D>& texture) {
		DrawQuad({position.x, position.y, 0.0f }, size, texture);
	}
} // EEngine::Renderer2D
