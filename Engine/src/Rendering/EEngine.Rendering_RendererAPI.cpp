module;
#include <glad/glad.h>

module EEngine.Rendering;
import :RendererAPI;

namespace EEngine::Rendering {
	void OpenGLRendererAPI::Initialize() const {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear(const Math::vec4& color) const {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Shared<VertexArray>& vertexArray, uint32_t indexCountOverride) const {
		vertexArray->Bind();

		uint32_t indexCount = indexCountOverride
			? indexCountOverride
			: vertexArray->GetIndexBuffer()->GetCount();

		glDrawElements(
			GL_TRIANGLES,
			(GLsizei)indexCount,
			GL_UNSIGNED_INT,
			nullptr
		);
	}

	Shared<IndexBuffer> OpenGLRendererAPI::CreateIndexBuffer(uint32_t* indices, uint32_t count) const {
		return MakeShared<OpenGLIndexBuffer>(indices, count);
	}

	Shared<VertexBuffer> OpenGLRendererAPI::CreateVertexBuffer(void* vertices, uint32_t size) const {
		return MakeShared<OpenGLVertexBuffer>(vertices, size);
	}

	Shared<Shader> OpenGLRendererAPI::CreateShader(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) const {
		auto expected = OpenGLShader::Create(name, vertexSource, fragmentSource);
		if (expected) { return *expected; }

		Log::CoreError("{}", expected.error());
		return nullptr;
	}

	Shared<Shader> OpenGLRendererAPI::CreateShader(const std::string& path) const {
		auto expected = OpenGLShader::Create(path);
		if (expected) { return *expected; }

		Log::CoreError("{}", expected.error());
		return nullptr;
	}

	Shared<VertexArray> OpenGLRendererAPI::CreateVertexArray() const {
		return MakeShared<OpenGLVertexArray>();
	}

	Shared<UniformBuffer> OpenGLRendererAPI::CreateUniformBuffer(uint32_t size, uint32_t binding) const {
		return MakeShared<OpenGLUniformBuffer>(size, binding);
	}

	Shared<Texture2D> OpenGLRendererAPI::CreateTexture2D(const std::string& path) const {
		auto expected = OpenGLTexture2D::Create(path);
		if (expected) { return *expected; }

		Log::CoreError("{}", expected.error());
		return nullptr;
	}

	Shared<Texture2D> OpenGLRendererAPI::CreateTexture2D(
		uint32_t width,
		uint32_t height,
		void* data,
		uint32_t size
	) const {
		return MakeShared<OpenGLTexture2D>(width, height, data, size);
	}

	bool OpenGLRendererAPI::TryGetOrLoadShader(const std::string& name, Shared<Shader>& outShader) {
		outShader = m_ShaderByName.contains(name)
			? m_ShaderByName[name]
			: CreateAndCacheShader(name);
		return outShader != nullptr;
	}

	Shared<Shader> OpenGLRendererAPI::CreateAndCacheShader(const std::string& path) {
		auto expected = OpenGLShader::Create(path);
		if (!expected) {
			Log::CoreError("{}", expected.error());
			return nullptr;
		}

		auto shader = *expected;
		const auto& name = shader->GetName();
		Log::CoreAssert(!m_ShaderByName.contains(name), "Tried to add duplicate shader.");
		m_ShaderByName[name] = shader;
		return shader;
	}
}
