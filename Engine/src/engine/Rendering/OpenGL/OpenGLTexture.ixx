module;
#include <glad/glad.h>
#include <stb_image.hpp>
#include "Core/Core.hpp"

export module EEngine.Rendering:OpenGLTexture;
import :ITexture;
import EEngine.Core;
import EEngine.std.core;

export namespace EEngine {
	class OpenGLTexture2D : public ITexture2D {
	public:
		explicit OpenGLTexture2D(const std::string& path) : m_Path(path) {
			int width, height, numChannels;

			stbi_set_flip_vertically_on_load(true);
			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
			EE_CORE_ASSERT(data, "Failed to load image.");
			m_Width = width;
			m_Height = height;

			GLenum storageFormat = GL_NONE, instanceFormat = GL_NONE;
			switch (numChannels) {
				case 4: {
					storageFormat = GL_RGBA8;
					instanceFormat = GL_RGBA;
					break;
				}
				case 3: {
					storageFormat = GL_RGB8;
					instanceFormat = GL_RGB;
					break;
				}
			}

			EE_CORE_ASSERT(storageFormat & instanceFormat, "Format not supported.");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, storageFormat, width, height);

			// ER TODO filter to be configurable in editor.
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureSubImage2D(
				m_RendererID,
				0,
				0,
				0,
				width,
				height,
				instanceFormat,
				GL_UNSIGNED_BYTE,
				data
			);

			stbi_image_free(data);
		}

		~OpenGLTexture2D() override {
			glDeleteTextures(1, &m_RendererID);
		}

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void Bind() const override {
			glBindTextureUnit(0, m_RendererID);
		}
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
} // EEngine
