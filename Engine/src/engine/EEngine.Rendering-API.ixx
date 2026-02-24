export module EEngine.Rendering:API;
import :Interfaces;
import :OpenGL;

using namespace EEngine;

namespace EEngine::RendererAPI {
	constexpr IRendererAPI& GetRendererAPI() {
		if constexpr (g_API == API::DirectX) {
			Log::CoreCritical("DirectX not implemented yet.");
			throw std::runtime_error("DirectX not implemented yet.");
		} else if constexpr (g_API == API::OpenGL) {
			static OpenGLRendererAPI instance;
			return instance;
		} else if constexpr (g_API == API::Vulkan) {
			Log::CoreCritical("Vulkan not implemented yet.");
			throw std::runtime_error("Vulkan not implemented yet.");
		} else {
			Log::CoreCritical("Unknown rendering API.");
			throw std::runtime_error("Unknown rendering API.");
		}
	}

	export {
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

		inline void Initialize() {
			GetRendererAPI().InitializeImpl();
		}

		inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			GetRendererAPI().SetViewportImpl(x, y, width, height);
		}

		inline void Clear(const Math::vec4& color) {
			GetRendererAPI().ClearImpl(color);
		}

		inline void DrawIndexed(const Shared<VertexArray>& vertexArray) {
			GetRendererAPI().DrawIndexedImpl(vertexArray);
		}

		inline Shared<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) {
			return GetRendererAPI().CreateIndexBufferImpl(indices, count);
		}

		inline Shared<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) {
			return GetRendererAPI().CreateVertexBufferImpl(vertices, size);
		}

		inline Shared<Shader> CreateShader(
			const std::string& name,
			const std::string& vertexSource,
			const std::string& fragmentSource
		) {
			return GetRendererAPI().CreateShaderImpl(name, vertexSource, fragmentSource);
		}

		inline Shared<Shader> CreateShader(const std::string& path) {
			return GetRendererAPI().CreateShaderImpl(path);
		}

		inline Shared<VertexArray> CreateVertexArray() {
			return GetRendererAPI().CreateVertexArrayImpl();
		}

		inline Shared<Texture2D> CreateTexture2D(const std::string& path) {
			return GetRendererAPI().CreateTexture2DImpl(path);
		}

		inline Shared<Texture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) {
			return GetRendererAPI().CreateTexture2DImpl(width, height, data, size);
		}
	}
}