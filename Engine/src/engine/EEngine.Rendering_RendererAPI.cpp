module;
#include <glad/glad.h>

module EEngine.Rendering;
import :RendererAPI;

namespace EEngine::Rendering {
	void OpenGLRendererAPI::Initialize() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear(const Math::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Shared<VertexArray>& vertexArray) {
		glDrawElements(
			GL_TRIANGLES,
			(GLsizei)vertexArray->GetIndexBuffer()->GetCount(),
			GL_UNSIGNED_INT,
			nullptr
		);
	}

	Shared<IndexBuffer> OpenGLRendererAPI::CreateIndexBuffer(uint32_t* indices, uint32_t count) {
		return MakeShared<OpenGLIndexBuffer>(indices, count);
	}

	Shared<VertexBuffer> OpenGLRendererAPI::CreateVertexBuffer(float* vertices, uint32_t size) {
		return MakeShared<OpenGLVertexBuffer>(vertices, size);
	}

	Shared<Shader> OpenGLRendererAPI::CreateShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) {
		return MakeShared<OpenGLShader>(name, vertexSource, fragmentSource);
	}

	Shared<Shader> OpenGLRendererAPI::CreateShader(const std::string& path) {
		return MakeShared<OpenGLShader>(path);
	}

	Shared<VertexArray> OpenGLRendererAPI::CreateVertexArray() {
		return MakeShared<OpenGLVertexArray>();
	}

	Shared<Texture2D> OpenGLRendererAPI::CreateTexture2D(const std::string& path) {
		return MakeShared<OpenGLTexture2D>(path);
	}

	Shared<Texture2D> OpenGLRendererAPI::CreateTexture2D(uint32_t width, uint32_t height, void* data, uint32_t size) {
		return MakeShared<OpenGLTexture2D>(width, height, data, size);
	}

	bool OpenGLRendererAPI::TryGetOrLoadShader(const std::string& name, Shared<Shader>& outShader) {
		outShader = m_ShaderByName.contains(name)
			? m_ShaderByName[name]
			: CreateAndCacheShader(name);
		return outShader != nullptr;
	}

	Shared<Shader> OpenGLRendererAPI::CreateAndCacheShader(const std::string& path) {
		Shared<Shader> shader = CreateShader(path);
		const auto& name = shader->GetName();
		Log::CoreAssert(!m_ShaderByName.contains(name), "Tried to add duplicate shader.");
		m_ShaderByName[name] = shader;
		return shader;
	}
}
