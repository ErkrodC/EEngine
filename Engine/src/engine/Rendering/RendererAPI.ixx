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
	static IRendererAPI* s_RendererAPI = new OpenGLRendererAPI();

	export inline void Initialize() {
		s_RendererAPI->InitializeImpl();
	}

	export inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		s_RendererAPI->SetViewportImpl(x, y, width, height);
	}

	export inline void Clear(const Math::vec4& color) {
		s_RendererAPI->ClearImpl(color);
	}

	export inline void DrawIndexed(const Ref<IVertexArray>& vertexArray) {
		s_RendererAPI->DrawIndexedImpl(vertexArray);
	}

	export inline Ref<IIndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) {
		return s_RendererAPI->CreateIndexBufferImpl(indices, count);
	}

	export inline Ref<IVertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) {
		return s_RendererAPI->CreateVertexBufferImpl(vertices, size);
	}

	export inline Ref<IShader> CreateShader(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) {
		return s_RendererAPI->CreateShaderImpl(name, vertexSource, fragmentSource);
	}

	export inline Ref<IShader> CreateShader(const std::string& path) {
		return s_RendererAPI->CreateShaderImpl(path);
	}

	export inline Ref<IVertexArray> CreateVertexArray() {
		return s_RendererAPI->CreateVertexArrayImpl();
	}

	export inline Ref<ITexture2D> CreateTexture2D(const std::string& path) {
		return s_RendererAPI->CreateTexture2DImpl(path);
	}
} // EEngine
