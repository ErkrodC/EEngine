export module EEngine.Rendering:Interfaces;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :Buffers;

using namespace EEngine;

export namespace EEngine::Rendering {
	enum class API {
		DirectX,
		OpenGL,
		Vulkan
	};

	constexpr API g_API = API::OpenGL;

	constexpr std::string GetRendererAPIString() {
		if constexpr (g_API == API::OpenGL) {
			return "OpenGL";
		} else if constexpr (g_API == API::DirectX) {
			return "DirectX";
		} else if constexpr (g_API == API::Vulkan) {
			return "Vulkan";
		} else {
			Log::CoreCritical("Unknown rendering API.");
			throw std::runtime_error("Unknown rendering API.");
		}
	}

	#define DECLARE_API_TYPE(BaseName) \
		class DirectX##BaseName; class OpenGL##BaseName; class Vulkan##BaseName; \
		using BaseName = std::conditional_t<g_API == API::DirectX, \
				DirectX##BaseName, \
				std::conditional_t<g_API == API::OpenGL, \
					OpenGL##BaseName, \
					Vulkan##BaseName \
			> \
		>

	// ============================================================================
	// Shader Type Alias & Concept
	// ============================================================================
	DECLARE_API_TYPE(Shader);
	template<typename T>
	concept ShaderConcept = requires(T shader, std::string name, int32_t i, float f, Math::vec2 v2, Math::vec3 v3, Math::vec4 v4, Math::mat3 m3, Math::mat4 m4) {
		{ shader.Bind() } -> std::same_as<void>;
		{ shader.Unbind() } -> std::same_as<void>;
		{ shader.SetInt(name, i) } -> std::same_as<void>;
		{ shader.SetFloat(name, f) } -> std::same_as<void>;
		{ shader.SetFloat2(name, v2) } -> std::same_as<void>;
		{ shader.SetFloat3(name, v3) } -> std::same_as<void>;
		{ shader.SetFloat4(name, v4) } -> std::same_as<void>;
		{ shader.SetMat3(name, m3) } -> std::same_as<void>;
		{ shader.SetMat4(name, m4) } -> std::same_as<void>;
		{ shader.GetName() } -> std::same_as<const std::string&>;
	};

	// ============================================================================
	// IndexBuffer Type Alias & Concept
	// ============================================================================
	DECLARE_API_TYPE(IndexBuffer);
	template<typename T>
	concept IndexBufferConcept = requires(T buffer) {
		{ buffer.Bind() } -> std::same_as<void>;
		{ buffer.Unbind() } -> std::same_as<void>;
		{ buffer.GetCount() } -> std::same_as<uint32_t>;
	};

	// ============================================================================
	// VertexBuffer Type Alias & Concept
	// ============================================================================
	DECLARE_API_TYPE(VertexBuffer);
	template<typename T>
	concept VertexBufferConcept = requires(T buffer, BufferLayout layout) {
		{ buffer.Bind() } -> std::same_as<void>;
		{ buffer.Unbind() } -> std::same_as<void>;
		{ buffer.GetLayout() } -> std::same_as<const BufferLayout&>;
		{ buffer.SetLayout(layout) } -> std::same_as<void>;
	};

	// ============================================================================
	// VertexArray Type Alias & Concept
	// ============================================================================
	DECLARE_API_TYPE(VertexArray);
	template<typename T>
	concept VertexArrayConcept = requires(T va, Shared<IndexBuffer> ib, Shared<VertexBuffer> vb) {
		{ va.Bind() } -> std::same_as<void>;
		{ va.Unbind() } -> std::same_as<void>;
		{ va.SetIndexBuffer(ib) } -> std::same_as<void>;
		{ va.AddVertexBuffer(vb) } -> std::same_as<void>;
		{ va.GetIndexBuffer() } -> std::same_as<const Shared<IndexBuffer>&>;
		{ va.GetVertexBuffers() } -> std::same_as<const std::vector<Shared<VertexBuffer>>&>;
	};

	// ============================================================================
	// Texture2D Type Alias & Concept
	// ============================================================================
	DECLARE_API_TYPE(Texture2D);
	template<typename T>
	concept Texture2DConcept = requires(T texture, void* data, uint32_t size) {
		{ texture.GetWidth() } -> std::same_as<uint32_t>;
		{ texture.GetHeight() } -> std::same_as<uint32_t>;
		{ texture.SetData(data, size) } -> std::same_as<void>;
		{ texture.Bind() } -> std::same_as<void>;
	};

	#undef DECLARE_API_TYPE
}
