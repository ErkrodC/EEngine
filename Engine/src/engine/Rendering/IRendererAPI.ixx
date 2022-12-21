export module EEngine.Rendering:IRendererAPI;
import :Buffer;
import :IShader;
import :IVertexArray;
import :ITexture;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;

export namespace EEngine {
	class IRendererAPI {
	public:
		virtual void InitializeImpl() = 0;
		virtual void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void ClearImpl(const Math::vec4& color) = 0;
		virtual void DrawIndexedImpl(const Ref<IVertexArray>& vertexArray) = 0;
		virtual Ref<IIndexBuffer> CreateIndexBufferImpl(uint32_t* indices, uint32_t count) = 0;
		virtual Ref<IVertexBuffer> CreateVertexBufferImpl(float* vertices, uint32_t size) = 0;
		virtual Ref<IShader> CreateShaderImpl(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) = 0;
		virtual Ref<IShader> CreateShaderImpl(const std::string& path) = 0;
		virtual Ref<IVertexArray> CreateVertexArrayImpl() = 0;
		virtual Ref<ITexture2D> CreateTexture2DImpl(const std::string& path) = 0;
		virtual Ref<ITexture2D> CreateTexture2DImpl(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) = 0;
	};
} // EEngine
