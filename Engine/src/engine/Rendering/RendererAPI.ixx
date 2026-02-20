export module EEngine.Rendering:RendererAPI;
import :Buffer;
import :IRendererAPI;
import :IShader;
import :IVertexArray;
import :ITexture;
import :OpenGLRendererAPI;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;

namespace EEngine::RendererAPI {
	inline IRendererAPI& GetRendererAPI() {
		static OpenGLRendererAPI instance;
		return instance;
	}

	export inline void Initialize() {
		GetRendererAPI().InitializeImpl();
	}

	export inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		GetRendererAPI().SetViewportImpl(x, y, width, height);
	}

	export inline void Clear(const Math::vec4& color) {
		GetRendererAPI().ClearImpl(color);
	}

	export inline void DrawIndexed(const Shared<IVertexArray>& vertexArray) {
		GetRendererAPI().DrawIndexedImpl(vertexArray);
	}

	export inline Shared<IIndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) {
		return GetRendererAPI().CreateIndexBufferImpl(indices, count);
	}

	export inline Shared<IVertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) {
		return GetRendererAPI().CreateVertexBufferImpl(vertices, size);
	}

	export inline Shared<IShader> CreateShader(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) {
		return GetRendererAPI().CreateShaderImpl(name, vertexSource, fragmentSource);
	}

	export inline Shared<IShader> CreateShader(const std::string& path) {
		return GetRendererAPI().CreateShaderImpl(path);
	}

	export inline Shared<IVertexArray> CreateVertexArray() {
		return GetRendererAPI().CreateVertexArrayImpl();
	}

	export inline Shared<ITexture2D> CreateTexture2D(const std::string& path) {
		return GetRendererAPI().CreateTexture2DImpl(path);
	}

	export inline Shared<ITexture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) {
		return GetRendererAPI().CreateTexture2DImpl(width, height, data, size);
	}
} // EEngine
