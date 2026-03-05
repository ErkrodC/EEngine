export module EEngine.Rendering:UniformBuffer;
import EEngine.Core;
import EEngine.Standard;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLUniformBuffer Implementation
	// ============================================================================
	class OpenGLUniformBuffer {
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		~OpenGLUniformBuffer();
		OpenGLUniformBuffer(const OpenGLUniformBuffer& other) = delete;
		OpenGLUniformBuffer& operator=(const OpenGLUniformBuffer& other) = delete;

		OpenGLUniformBuffer(OpenGLUniformBuffer&& other) noexcept;
		OpenGLUniformBuffer& operator=(OpenGLUniformBuffer&& other) noexcept;

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) const;
	private:
		uint32_t m_RendererID = 0;
	};

	// ============================================================================
	// UniformBuffer Type Alias
	// ============================================================================
	class DirectXUniformBuffer; 
	class VulkanUniformBuffer; 
	using UniformBuffer = std::conditional_t<g_API == API::DirectX, 
		DirectXUniformBuffer, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLUniformBuffer, 
			VulkanUniformBuffer 
		> 
	>;

	// ============================================================================
	// UniformBuffer Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept UniformBufferConcept = requires(T buffer, const void* data, uint32_t size, uint32_t offset) {
		{ buffer.SetData(data, size, offset) } -> std::same_as<void>;
	};
	static_assert(UniformBufferConcept<UniformBuffer>);
}
