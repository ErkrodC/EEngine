module;
#include <glad/glad.h>

export module EEngine.Rendering:RendererAPI;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :API;
import :IndexBuffer;
import :VertexBuffer;
import :VertexArray;
import :Shader;
import :Texture;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLRendererAPI Implementation
	// ============================================================================
	class OpenGLRendererAPI {
	public:
		void Initialize() {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			glViewport(x, y, width, height);
		}

		void Clear(const Math::vec4& color) {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void DrawIndexed(const Shared<VertexArray>& vertexArray) {
			glDrawElements(GL_TRIANGLES,
				(GLsizei)vertexArray->GetIndexBuffer()->GetCount(),
				GL_UNSIGNED_INT,
				nullptr
			);
		}

		Shared<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) {
			return MakeShared<OpenGLIndexBuffer>(indices, count);
		}

		Shared<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) {
			return MakeShared<OpenGLVertexBuffer>(vertices, size);
		}

		Shared<Shader> CreateShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) {
			return MakeShared<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		Shared<Shader> CreateShader(const std::string& path) {
			return MakeShared<OpenGLShader>(path);
		}

		Shared<VertexArray> CreateVertexArray() {
			return MakeShared<OpenGLVertexArray>();
		}

		Shared<Texture2D> CreateTexture2D(const std::string& path) {
			return MakeShared<OpenGLTexture2D>(path);
		}

		Shared<Texture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) {
			return MakeShared<OpenGLTexture2D>(width, height, data, size);
		}

		bool TryGetOrLoadShader(const std::string& name, Shared<Shader>& outShader) {
			outShader = m_ShaderByName.contains(name)
				? m_ShaderByName[name]
				: CreateAndCacheShader(name);
			return outShader != nullptr;
		}

	private:
		std::unordered_map<std::string, Shared<Shader>> m_ShaderByName;

		Shared<Shader> CreateAndCacheShader(const std::string& path) {
			Shared<Shader> shader = CreateShader(path);
			auto& name = shader->GetName();
			Log::CoreAssert(!m_ShaderByName.contains(name), "Tried to add duplicate shader.");
			m_ShaderByName[name] = shader;
			return shader;
		}
	};

	// ============================================================================
	// RendererAPI Type Alias
	// ============================================================================
	class DirectXRendererAPI; 
	class VulkanRendererAPI; 
	using RendererAPI = std::conditional_t<g_API == API::DirectX, 
		DirectXRendererAPI, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLRendererAPI, 
			VulkanRendererAPI 
		> 
	>;

	// ============================================================================
	// RendererAPI Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept RendererAPIConcept = requires(
		T api,
		uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t count, uint32_t size, uint32_t* indices,
		float* vertices,
		Math::vec4 color,
		std::string path, std::string name, std::string vertexSource, std::string fragmentSource,
		Shared<Shader> shader
	) {
		{ api.Initialize() } -> std::same_as<void>;
		{ api.SetViewport(x, y, x, y) } -> std::same_as<void>;
		{ api.Clear(color) } -> std::same_as<void>;
		{ api.DrawIndexed(Shared<VertexArray>{}) } -> std::same_as<void>;
		{ api.CreateIndexBuffer(indices, count) } -> std::same_as<Shared<IndexBuffer>>;
		{ api.CreateVertexBuffer(vertices, size) } -> std::same_as<Shared<VertexBuffer>>;
		{ api.CreateShader(name, vertexSource, fragmentSource) } -> std::same_as<Shared<Shader>>;
		{ api.CreateShader(path) } -> std::same_as<Shared<Shader>>;
		{ api.CreateVertexArray() } -> std::same_as<Shared<VertexArray>>;
		{ api.CreateTexture2D(path) } -> std::same_as<Shared<Texture2D>>;
		{ api.CreateTexture2D(width, height, vertices, size) } -> std::same_as<Shared<Texture2D>>;
		{ api.TryGetOrLoadShader(path, shader) } -> std::same_as<bool>;
	};
	static_assert(RendererAPIConcept<RendererAPI>);
}
