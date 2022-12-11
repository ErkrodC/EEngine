module;
#include <string>
#include <glad/glad.h>
#include <glm/vec4.hpp>

module EEngine.Rendering:OpenGLRendererAPI;
import :Buffer;
import :OpenGLBuffer;
import :IShader;
import :IVertexArray;
import :ITexture;
import EEngine.Core;

namespace EEngine {
	void OpenGLRendererAPI::InitializeImpl() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::ClearImpl(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexedImpl(const Ref<IVertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES,
			(GLsizei)vertexArray->GetIndexBuffer()->GetCount(),
			GL_UNSIGNED_INT,
			nullptr
		);
	}

	Ref<IIndexBuffer> OpenGLRendererAPI::CreateIndexBufferImpl(
		uint32_t* indices,
		uint32_t count
	) {
		return std::make_shared<OpenGLIndexBuffer>(indices, count);
	}

	Ref<IVertexBuffer> OpenGLRendererAPI::CreateVertexBufferImpl(
		float* vertices,
		uint32_t size
	) {
		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
	}

	Ref<IShader> OpenGLRendererAPI::CreateShaderImpl(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) {
		return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
	}

	Ref<IShader> OpenGLRendererAPI::CreateShaderImpl(const std::string& path) {
		return std::make_shared<OpenGLShader>(path);
	}

	Ref<IVertexArray> OpenGLRendererAPI::CreateVertexArrayImpl() {
		return std::make_shared<OpenGLVertexArray>();
	}

	Ref<ITexture2D> OpenGLRendererAPI::CreateTexture2D(const std::string& path) {
		return std::make_shared<OpenGLTexture2D>(path);
	}
} // EEngine