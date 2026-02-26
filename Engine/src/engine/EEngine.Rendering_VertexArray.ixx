module;
#include <glad/glad.h>
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
		OpenGLVertexArray() { glCreateVertexArrays(1, &m_RendererID); }
		~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }
		OpenGLVertexArray(const OpenGLVertexArray& other) = delete;
		OpenGLVertexArray& operator=(const OpenGLVertexArray& other) = delete;

		OpenGLVertexArray(OpenGLVertexArray&& other) noexcept
			: m_RendererID(std::exchange(other.m_RendererID, 0)),
			  m_IndexBuffer(std::move(other.m_IndexBuffer)),
			  m_VertexBuffers(std::move(other.m_VertexBuffers)) {}

		OpenGLVertexArray& operator=(OpenGLVertexArray&& other) noexcept {
			glDeleteVertexArrays(1, &m_RendererID);
			m_RendererID = 0;

			m_RendererID = std::exchange(other.m_RendererID, 0);
			m_IndexBuffer = std::move(other.m_IndexBuffer);
			m_VertexBuffers = std::move(other.m_VertexBuffers);
			return *this;
		}

		void Bind() const {
			glBindVertexArray(m_RendererID);
		}

		void Unbind() const {
			glBindVertexArray(0);
		}

		void SetIndexBuffer(const Shared<IndexBuffer>& indexBuffer) {
			glBindVertexArray(m_RendererID);
			indexBuffer->Bind();

			m_IndexBuffer = indexBuffer;
		}

		void AddVertexBuffer(const Shared<VertexBuffer>& vertexBuffer) {
			Log::CoreAssert(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout.");

			glBindVertexArray(m_RendererID);
			vertexBuffer->Bind();

			uint32_t index = 0;
			const auto& layout = vertexBuffer->GetLayout();
			for (const auto& element : layout) {
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(
					index++,
					(GLint)element.ComponentCount,
					ShaderDataToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					(GLsizei)layout.GetStride(),
					(const void *)(intptr_t)element.Offset
				);
			}

			m_VertexBuffers.push_back(vertexBuffer);
		}

		const Shared<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
		const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
	private:
		uint32_t m_RendererID;
		Shared<IndexBuffer> m_IndexBuffer;
		std::vector<Shared<VertexBuffer>> m_VertexBuffers;

		static GLenum ShaderDataToOpenGLBaseType(ShaderData type) {
			switch (type) {
				case ShaderData::None:		break;

				case ShaderData::Float:
				case ShaderData::Float2:
				case ShaderData::Float3:
				case ShaderData::Float4:
				case ShaderData::Mat3:
				case ShaderData::Mat4:		return GL_FLOAT;

				case ShaderData::Int:
				case ShaderData::Int2:
				case ShaderData::Int3:
				case ShaderData::Int4:			return GL_INT;

				case ShaderData::Bool:			return GL_BOOL;
			}

			Log::CoreError("Unknown shader data type.");
			return GL_NONE;
		}
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
