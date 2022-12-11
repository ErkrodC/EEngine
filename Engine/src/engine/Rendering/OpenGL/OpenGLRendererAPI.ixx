module;
#include <string>
#include "glm/vec4.hpp"

export module EEngine.Rendering:OpenGLRendererAPI;
import :Buffer;
import :IRendererAPI;
import :IShader;
import :IVertexArray;
import :ITexture;
import EEngine.Core;

export namespace EEngine {
	class OpenGLRendererAPI : public IRendererAPI {
	/*public:
	private:*/
	public:
		void InitializeImpl() override;
		void ClearImpl(const glm::vec4& color) override;
		void DrawIndexedImpl(const Ref<IVertexArray>& vertexArray) override;
		Ref<IIndexBuffer> CreateIndexBufferImpl(uint32_t* indices, uint32_t count) override;
		Ref<IVertexBuffer> CreateVertexBufferImpl(float* vertices, uint32_t size) override;
		Ref<IShader> CreateShaderImpl(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) override;
		Ref<IShader> CreateShaderImpl(const std::string& path) override;
		Ref<IVertexArray> CreateVertexArrayImpl() override;
		Ref<ITexture2D> CreateTexture2DImpl(const std::string& path) override;
	};
} // EEngine
