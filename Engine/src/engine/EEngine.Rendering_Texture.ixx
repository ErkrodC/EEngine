module;
#include <glad/glad.h>
#include <stb_image.hpp>

export module EEngine.Rendering:Texture;
import EEngine.Core;
import EEngine.Standard;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLTexture2D Implementation
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
	// Texture2D Type Alias
	// ============================================================================
	class DirectXTexture2D; 
	class VulkanTexture2D; 
	using Texture2D = std::conditional_t<g_API == API::DirectX, 
		DirectXTexture2D, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLTexture2D, 
			VulkanTexture2D 
		> 
	>;

	// ============================================================================
	// Texture2D Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept Texture2DConcept = requires(T texture, void* data, uint32_t size) {
		{ texture.GetWidth() } -> std::same_as<uint32_t>;
		{ texture.GetHeight() } -> std::same_as<uint32_t>;
		{ texture.SetData(data, size) } -> std::same_as<void>;
		{ texture.Bind() } -> std::same_as<void>;
	};
	static_assert(Texture2DConcept<Texture2D>);
}
