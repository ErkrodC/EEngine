module;
#include <glad/glad.h>

export module EEngine.Rendering:OpenGLRendererAPI;
import :Buffer;
import :IRendererAPI;
import :IShader;
import :IVertexArray;
import :ITexture;
import :OpenGLBuffer;
import :OpenGLShader;
import :OpenGLVertexArray;
import :OpenGLTexture;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;

export namespace EEngine {
	class OpenGLRendererAPI : public IRendererAPI {
	public:
		void InitializeImpl() override {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glEnable(GL_DEPTH_TEST);
		}

		void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override {
			glViewport(x, y, width, height);
		}

		void ClearImpl(const Math::vec4& color) override {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);
		}

		void DrawIndexedImpl(const Ref<IVertexArray>& vertexArray) override {
			glDrawElements(GL_TRIANGLES,
				(GLsizei)vertexArray->GetIndexBuffer()->GetCount(),
				GL_UNSIGNED_INT,
				nullptr
			);
		}

		Ref<IIndexBuffer> CreateIndexBufferImpl(uint32_t* indices, uint32_t count) override {
			return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		Ref<IVertexBuffer> CreateVertexBufferImpl(float* vertices, uint32_t size) override {
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		Ref<IShader> CreateShaderImpl(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) override {
			return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		Ref<IShader> CreateShaderImpl(const std::string& path) override {
			return std::make_shared<OpenGLShader>(path);
		}

		Ref<IVertexArray> CreateVertexArrayImpl() override {
			return std::make_shared<OpenGLVertexArray>();
		}

		Ref<ITexture2D> CreateTexture2DImpl(const std::string& path) override {
			return std::make_shared<OpenGLTexture2D>(path);
		}
	};
} // EEngine
