module;
#include <string>

export module EEngine.Rendering:OpenGLTexture;
import :ITexture;

export namespace EEngine {
	class OpenGLTexture2D : public ITexture2D {
	public:
		explicit OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D() override;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		void Bind() const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
} // EEngine
