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

	void OpenGLRendererAPI::CreateShadowMap(uint32_t resolution) {
		m_ShadowMapResolution = resolution;

		glGenFramebuffers(1, &m_ShadowFBO);

		glGenTextures(1, &m_ShadowDepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_ShadowDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float_t borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// Enable hardware PCF (percentage-closer filtering)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowDepthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRendererAPI::BindShadowFrameBuffer() const {
		glViewport(0, 0, m_ShadowMapResolution, m_ShadowMapResolution);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::UnbindShadowFrameBuffer() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRendererAPI::BindShadowTexture(uint32_t slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ShadowDepthTexture);
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

	bool OpenGLRendererAPI::TryGetOrLoadShader(const std::string& path, Shared<Shader>& outShader) {
		std::string name = GetNameFromPath(path);

		outShader = m_ShaderByName.contains(name)
			? m_ShaderByName[name]
			: CreateAndCacheShader(path);
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
