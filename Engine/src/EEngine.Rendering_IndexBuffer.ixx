export module EEngine.Rendering:IndexBuffer;
import EEngine.Core;
import EEngine.Standard;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLIndexBuffer Implementation
	// ============================================================================
	class OpenGLIndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();
		OpenGLIndexBuffer(const OpenGLIndexBuffer& other) = delete;
		OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer& other) = delete;

		OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept;
		OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& other) noexcept;

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	// ============================================================================
	// IndexBuffer Type Alias
	// ============================================================================
	class DirectXIndexBuffer; 
	class VulkanIndexBuffer; 
	using IndexBuffer = std::conditional_t<g_API == API::DirectX, 
		DirectXIndexBuffer, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLIndexBuffer, 
			VulkanIndexBuffer 
		> 
	>;

	// ============================================================================
	// IndexBuffer Concept and Static Assert
	// ============================================================================
	template<typename TIndexBuffer>
	concept IndexBufferConcept = requires(TIndexBuffer buffer, uint32_t* indices, uint32_t count) {
		{ TIndexBuffer(indices, count) } -> std::same_as<TIndexBuffer>;
		{ buffer.Bind() } -> std::same_as<void>;
		{ buffer.Unbind() } -> std::same_as<void>;
		{ buffer.GetCount() } -> std::same_as<uint32_t>;
	};
	static_assert(IndexBufferConcept<IndexBuffer>);
}
