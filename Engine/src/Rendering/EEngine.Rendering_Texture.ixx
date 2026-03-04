export module EEngine.Rendering:Texture;
import EEngine.Core;
import EEngine.Standard;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// Texture2D Type Alias
	// ============================================================================
	class DirectXTexture2D;
	class VulkanTexture2D;
	class OpenGLTexture2D;
	using Texture2D = std::conditional_t<g_API == API::DirectX,
		DirectXTexture2D,
		std::conditional_t<g_API == API::OpenGL,
			OpenGLTexture2D,
			VulkanTexture2D
		>
	>;

	// ============================================================================
	// OpenGLTexture2D Implementation
	// ============================================================================
	class OpenGLTexture2D {
	public:
		static Expected<Shared<Texture2D>, std::string> Create(const std::string& path);

		explicit OpenGLTexture2D(uint32_t width, uint32_t height, void* data, uint32_t size);

		~OpenGLTexture2D();
		OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D& other) = delete;

		OpenGLTexture2D(OpenGLTexture2D&& other) noexcept;
		OpenGLTexture2D& operator=(OpenGLTexture2D&& other) noexcept;

		void Bind() const;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		uint32_t m_InstanceFormat;

		uint32_t GetBytesPerPixel() const;
		void SetData(void* data, uint32_t size) const;
	};

	// ============================================================================
	// Texture2D Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept Texture2DConcept = requires(T texture, void* data, uint32_t size) {
		{ texture.Bind() } -> std::same_as<void>;
	};
	static_assert(Texture2DConcept<Texture2D>);
}
