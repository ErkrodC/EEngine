module;
#include <glad/glad.h>
#include <stb_image.hpp>

export module EEngine.Rendering:OpenGLTexture;
import :ITexture;
import EEngine.Core;
import EEngine.Standard;

export namespace EEngine {
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

			// ER TODO filter to be configurable in editor.
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);

			if (!data) {
				// default to 1x1 white texture
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

			// ER TODO filter to be configurable in editor.
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);

			SetData(data, m_Width * m_Height * GetBytesPerPixel());

			stbi_image_free(data);
		}

		~OpenGLTexture2D() override {
			glDeleteTextures(1, &m_RendererID);
		}

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

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
} // EEngine
