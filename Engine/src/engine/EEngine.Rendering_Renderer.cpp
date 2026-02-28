module EEngine.Rendering;
import :Renderer;

namespace EEngine::Rendering {
	Renderer::Renderer(RendererAPI& rendererAPI)
		: m_RendererAPI(rendererAPI)
		, m_QuadVertexBufferPtr(m_QuadVertexBufferBase)
	{
		for (uint32_t i = 0, j = 0; i < MAX_QUAD_INDEX_BUFFER_COUNT; i += 6, j += 4) {
			m_QuadIndexBufferPtr[i + 0] = j + 0;
			m_QuadIndexBufferPtr[i + 1] = j + 1;
			m_QuadIndexBufferPtr[i + 2] = j + 2;
			m_QuadIndexBufferPtr[i + 3] = j + 2;
			m_QuadIndexBufferPtr[i + 4] = j + 3;
			m_QuadIndexBufferPtr[i + 5] = j + 0;
		}

		if constexpr (g_API == API::DirectX) {
			Log::CoreCritical("DirectX Renderer not implemented yet.");
			throw std::runtime_error("DirectX Renderer not implemented yet.");
		} else if constexpr (g_API == API::OpenGL) {
			m_Data.QuadVertexArray = m_RendererAPI.CreateVertexArray();
			uint32_t whiteTextureData = 0xffffffff;
			m_Data.WhiteTexture = m_RendererAPI.CreateTexture2D(1, 1, &whiteTextureData, sizeof(uint32_t));

			Shared<VertexBuffer> quadVertexBuffer = m_RendererAPI.CreateVertexBuffer(m_QuadVertexBufferPtr, MAX_QUAD_VERTEX_BUFFER_SIZE);
			quadVertexBuffer->SetLayout({
				{"a_Position", ShaderData::Float3},
				{"a_Color",    ShaderData::Float4},
				{"a_TexCoord", ShaderData::Float2}
			});
			m_Data.QuadVertexArray->AddVertexBuffer(quadVertexBuffer);
			m_Data.QuadVertexArray->SetIndexBuffer(m_RendererAPI.CreateIndexBuffer(m_QuadIndexBufferPtr, MAX_QUAD_INDEX_BUFFER_COUNT));

			if (m_RendererAPI.TryGetOrLoadShader("assets/shaders/Texture.glsl", m_Data.TextureShader)) {
				m_Data.TextureShader->Bind();
				m_Data.TextureShader->SetInt("u_Texture", 0);
			} else {
				Log::CoreCritical("Failed to load texture shader.");
				throw std::runtime_error("Failed to load texture shader.");
			}
		} else if constexpr (g_API == API::Vulkan) {
			Log::CoreCritical("Vulkan Renderer not implemented yet.");
			throw std::runtime_error("Vulkan Renderer not implemented yet.");
		} else {
			Log::CoreCritical("Unknown rendering API.");
			throw std::runtime_error("Unknown rendering API.");
		}
	}

	void Renderer::DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) {
		if (GetQuadVertexCount() >= MAX_QUAD_VERTEX_BUFFER_COUNT || m_Data.CurrentTexture != m_Data.WhiteTexture) {
			Flush();
		}

		m_Data.CurrentTexture = m_Data.WhiteTexture;
		CreateQuadVertices(position, size, color);
	}

	void Renderer::DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<Texture2D>& texture, const Math::vec4& tint) {
		if (GetQuadVertexCount() >= MAX_QUAD_VERTEX_BUFFER_COUNT || m_Data.CurrentTexture != texture) {
			Flush();
		}

		m_Data.CurrentTexture = texture;
		CreateQuadVertices(position, size, tint);
	}

	void Renderer::DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) {
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer::DrawQuad(const Math::vec2& position, const Math::vec2& size, const Shared<Texture2D>& texture) {
		DrawQuad({position.x, position.y, 0.0f}, size, texture);
	}

	void Renderer::BeginScene(const Math::mat4& projectionView) {
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;

		m_Data.TextureShader->Bind();
		m_Data.TextureShader->SetMat4("u_ProjectionView", projectionView);
	}

	void Renderer::EndScene() {
		Flush();
	}

	void Renderer::CreateQuadVertices(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) {
		QuadVertex& vertex0 = m_QuadVertexBufferPtr[0];
		QuadVertex& vertex1 = m_QuadVertexBufferPtr[1];
		QuadVertex& vertex2 = m_QuadVertexBufferPtr[2];
		QuadVertex& vertex3 = m_QuadVertexBufferPtr[3];

		float xHalfLength = size.x * 0.5f;
		float yHalfLength = size.y * 0.5f;
		vertex0.Position = { position.x - xHalfLength, position.y - yHalfLength, position.z };
		vertex1.Position = { position.x + xHalfLength, position.y - yHalfLength, position.z };
		vertex2.Position = { position.x + xHalfLength, position.y + yHalfLength, position.z };
		vertex3.Position = { position.x - xHalfLength, position.y + yHalfLength, position.z };

		vertex0.Color = vertex1.Color = vertex2.Color = vertex3.Color = color;

		vertex0.TexCoord = { 0.0f, 0.0f };
		vertex1.TexCoord = { 1.0f, 0.0f };
		vertex2.TexCoord = { 1.0f, 1.0f };
		vertex3.TexCoord = { 0.0f, 1.0f };

		m_QuadVertexBufferPtr += 4;
	}

	void Renderer::Flush() {
		if (GetQuadVertexCount() == 0) { return; }

		uint32_t dataSize = (uint32_t)((uint8_t*)m_QuadVertexBufferPtr - (uint8_t*)m_QuadVertexBufferBase);
		m_Data.QuadVertexArray->GetVertexBuffers().front()->SetData(m_QuadVertexBufferBase, dataSize);

		if (m_Data.CurrentTexture) { m_Data.CurrentTexture->Bind(); }
		else								{ m_Data.WhiteTexture->Bind(); }

		m_RendererAPI.DrawIndexed(m_Data.QuadVertexArray, GetQuadIndexCount());
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
	}

}
