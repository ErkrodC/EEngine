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
		explicit OpenGLTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0);
		explicit OpenGLTexture2D(const std::string& path);

		~OpenGLTexture2D();
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D& other) = delete;

		OpenGLTexture2D(OpenGLTexture2D&& other) noexcept;
		OpenGLTexture2D& operator=(OpenGLTexture2D&& other) noexcept;

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		void SetData(void* data, uint32_t size);

		void Bind() const;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		uint32_t m_InstanceFormat;

		uint32_t GetBytesPerPixel() const;
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
