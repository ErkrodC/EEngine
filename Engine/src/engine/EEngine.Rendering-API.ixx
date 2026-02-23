export module EEngine.Rendering:API;
import :OpenGL;

using namespace EEngine;

namespace EEngine::RendererAPI {
	inline IRendererAPI& GetRendererAPI() {
		static OpenGLRendererAPI instance;
		return instance;
	}

	export {
		inline void Initialize() {
			GetRendererAPI().InitializeImpl();
		}

		inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			GetRendererAPI().SetViewportImpl(x, y, width, height);
		}

		inline void Clear(const Math::vec4& color) {
			GetRendererAPI().ClearImpl(color);
		}

		inline void DrawIndexed(const Shared<IVertexArray>& vertexArray) {
			GetRendererAPI().DrawIndexedImpl(vertexArray);
		}

		inline Shared<IIndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) {
			return GetRendererAPI().CreateIndexBufferImpl(indices, count);
		}

		inline Shared<IVertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) {
			return GetRendererAPI().CreateVertexBufferImpl(vertices, size);
		}

		inline Shared<IShader> CreateShader(
			const std::string& name,
			const std::string& vertexSource,
			const std::string& fragmentSource
		) {
			return GetRendererAPI().CreateShaderImpl(name, vertexSource, fragmentSource);
		}

		inline Shared<IShader> CreateShader(const std::string& path) {
			return GetRendererAPI().CreateShaderImpl(path);
		}

		inline Shared<IVertexArray> CreateVertexArray() {
			return GetRendererAPI().CreateVertexArrayImpl();
		}

		inline Shared<ITexture2D> CreateTexture2D(const std::string& path) {
			return GetRendererAPI().CreateTexture2DImpl(path);
		}

		inline Shared<ITexture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) {
			return GetRendererAPI().CreateTexture2DImpl(width, height, data, size);
		}
	}
}