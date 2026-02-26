module;
#include <cstdint>

export module EEngine.Rendering:VertexArray;
import EEngine.Core;
import EEngine.Standard;
import :API;
import :Buffers;
import :IndexBuffer;
import :VertexBuffer;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLVertexArray Implementation
	// ============================================================================
	class OpenGLVertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		OpenGLVertexArray(const OpenGLVertexArray& other) = delete;
		OpenGLVertexArray& operator=(const OpenGLVertexArray& other) = delete;

		OpenGLVertexArray(OpenGLVertexArray&& other) noexcept;
		OpenGLVertexArray& operator=(OpenGLVertexArray&& other) noexcept;

		void Bind() const;
		void Unbind() const;

		void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer);
		void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer);

		const Shared<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
		const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
	private:
		uint32_t m_RendererID;
		Shared<IndexBuffer> m_IndexBuffer;
		std::vector<Shared<VertexBuffer>> m_VertexBuffers;
	};

	// ============================================================================
	// VertexArray Type Alias
	// ============================================================================
	class DirectXVertexArray; 
	class VulkanVertexArray; 
	using VertexArray = std::conditional_t<g_API == API::DirectX, 
		DirectXVertexArray, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLVertexArray, 
			VulkanVertexArray 
		> 
	>;

	// ============================================================================
	// VertexArray Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept VertexArrayConcept = requires(T va, Shared<IndexBuffer> ib, Shared<VertexBuffer> vb) {
		{ va.Bind() } -> std::same_as<void>;
		{ va.Unbind() } -> std::same_as<void>;
		{ va.SetIndexBuffer(ib) } -> std::same_as<void>;
		{ va.AddVertexBuffer(vb) } -> std::same_as<void>;
		{ va.GetIndexBuffer() } -> std::same_as<const Shared<IndexBuffer>&>;
		{ va.GetVertexBuffers() } -> std::same_as<const std::vector<Shared<VertexBuffer>>&>;
	};
	static_assert(VertexArrayConcept<VertexArray>);
}
