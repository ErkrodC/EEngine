module;
#include <glad/glad.h>
#include <stb_image.hpp>

module EEngine.Rendering;
import :Texture;

namespace EEngine::Rendering {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void* data, uint32_t size)
		: m_Width(width)
		, m_Height(height)
	{
		GLenum storageFormat = GL_RGBA8;
		m_InstanceFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, storageFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		SetData(data, size);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
		int width, height, numChannels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
		Log::CoreAssert(data, "Failed to load image.");
		m_Width = (uint32_t)width;
		m_Height = (uint32_t)height;

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
		glTextureStorage2D(m_RendererID, 1, storageFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		SetData(data, m_Width * m_Height * GetBytesPerPixel());

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

	OpenGLTexture2D::OpenGLTexture2D(OpenGLTexture2D&& other) noexcept
		: m_Path(std::move(other.m_Path)),
		  m_Width(std::move(other.m_Width)),
		  m_Height(std::move(other.m_Height)),
		  m_RendererID(std::exchange(other.m_RendererID, 0)),
		  m_InstanceFormat(std::move(other.m_InstanceFormat)) {}

	OpenGLTexture2D& OpenGLTexture2D::operator=(OpenGLTexture2D&& other) noexcept {
		glDeleteTextures(1, &m_RendererID);
		m_RendererID = 0;

		m_Path = std::move(other.m_Path);
		m_Width = std::move(other.m_Width);
		m_Height = std::move(other.m_Height);
		m_RendererID = std::exchange(other.m_RendererID, 0);
		m_InstanceFormat = std::move(other.m_InstanceFormat);

		return *this;
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		if (size) {
			uint32_t expectedSize = m_Width * m_Height * GetBytesPerPixel();
			Log::CoreAssert(
				size == expectedSize,
				"Incorrect size when setting texture data. Expected {}, got {}",
				expectedSize,
				size
			);
		}

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

	void OpenGLTexture2D::Bind() const {
		glBindTextureUnit(0, m_RendererID);
	}

	uint32_t OpenGLTexture2D::GetBytesPerPixel() const {
		switch (m_InstanceFormat) {
			case GL_RGBA: return 4;
			case GL_RGB: return 3;

			default:
				Log::CoreError("Unsupported texture instance format.");
				return 0;
		}
	}
}
