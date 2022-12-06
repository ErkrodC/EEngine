#include "OpenGLTexture.hpp"
#include <glad/glad.h>
#include "stb_image.hpp"

namespace EEngine {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path) {
		int width, height, numChannels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
		EE_CORE_ASSERT(data, "Failed to load image.");

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA32F, width, height);

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
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data
		);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind() const {
		glBindTextureUnit(0, m_RendererID);
	}
} // EEngine