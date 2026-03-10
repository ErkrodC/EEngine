module;
#include <glm/detail/type_mat4x4.hpp>

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
			// --- Quad VAO ---
			m_Data.QuadVertexArray = m_RendererAPI.CreateVertexArray();
			uint32_t whiteTextureData = 0xffffffff;
			m_Data.WhiteTexture = m_RendererAPI.CreateTexture2D(1, 1, &whiteTextureData, sizeof(uint32_t));

			Shared<VertexBuffer> quadVertexBuffer = m_RendererAPI.CreateVertexBuffer(m_QuadVertexBufferPtr, MAX_QUAD_VERTEX_BUFFER_SIZE);
			quadVertexBuffer->SetLayout({
				{"a_Color",    ShaderData::Float4},
				{"a_Normal",   ShaderData::Float3},
				{"a_Position", ShaderData::Float3},
				{"a_TexCoord", ShaderData::Float2}
			});
			m_Data.QuadVertexArray->AddVertexBuffer(quadVertexBuffer);
			m_Data.QuadVertexArray->SetIndexBuffer(m_RendererAPI.CreateIndexBuffer(m_QuadIndexBufferPtr, MAX_QUAD_INDEX_BUFFER_COUNT));

			// --- Cube VAO (24 verts, 36 indices, with face normals) ---
			float_t w = 1.0f; // white vertex color - tinted per-draw via u_Tint
			Vertex cubeVertices[] = {
				// Front (z=+0.5, normal 0,0,1)
				{{w,w,w,w}, {0,0,1}, {-0.5f,-0.5f,0.5f}, {0,0}},
				{{w,w,w,w}, {0,0,1}, { 0.5f,-0.5f,0.5f}, {1,0}},
				{{w,w,w,w}, {0,0,1}, { 0.5f, 0.5f,0.5f}, {1,1}},
				{{w,w,w,w}, {0,0,1}, {-0.5f, 0.5f,0.5f}, {0,1}},

				// Back (z=-0.5, normal 0,0,-1)
				{{w,w,w,w}, {0,0,-1}, { 0.5f,-0.5f,-0.5f}, {0,0}},
				{{w,w,w,w}, {0,0,-1}, {-0.5f,-0.5f,-0.5f}, {1,0}},
				{{w,w,w,w}, {0,0,-1}, {-0.5f, 0.5f,-0.5f}, {1,1}},
				{{w,w,w,w}, {0,0,-1}, { 0.5f, 0.5f,-0.5f}, {0,1}},

				// Top (y=+0.5, normal 0,1,0)
				{{w,w,w,w}, {0,1,0}, {-0.5f, 0.5f, 0.5f}, {0,0}},
				{{w,w,w,w}, {0,1,0}, { 0.5f, 0.5f, 0.5f}, {1,0}},
				{{w,w,w,w}, {0,1,0}, { 0.5f, 0.5f,-0.5f}, {1,1}},
				{{w,w,w,w}, {0,1,0}, {-0.5f, 0.5f,-0.5f}, {0,1}},

				// Bottom (y=-0.5, normal 0,-1,0)
				{{w,w,w,w}, {0,-1,0}, {-0.5f,-0.5f,-0.5f}, {0,0}},
				{{w,w,w,w}, {0,-1,0}, { 0.5f,-0.5f,-0.5f}, {1,0}},
				{{w,w,w,w}, {0,-1,0}, { 0.5f,-0.5f, 0.5f}, {1,1}},
				{{w,w,w,w}, {0,-1,0}, {-0.5f,-0.5f, 0.5f}, {0,1}},

				// Right (x=+0.5, normal 1,0,0)
				{{w,w,w,w}, { 1, 0, 0}, { 0.5f,-0.5f, 0.5f}, {0,0}},
				{{w,w,w,w}, { 1, 0, 0}, { 0.5f,-0.5f,-0.5f}, {1,0}},
				{{w,w,w,w}, { 1, 0, 0}, { 0.5f, 0.5f,-0.5f}, {1,1}},
				{{w,w,w,w}, { 1, 0, 0}, { 0.5f, 0.5f, 0.5f}, {0,1}},

				// Left (x=-0.5, normal -1,0,0)
				{{w,w,w,w}, {-1, 0, 0}, {-0.5f,-0.5f,-0.5f}, {0,0}},
				{{w,w,w,w}, {-1, 0, 0}, {-0.5f,-0.5f, 0.5f}, {1,0}},
				{{w,w,w,w}, {-1, 0, 0}, {-0.5f, 0.5f, 0.5f}, {1,1}},
				{{w,w,w,w}, {-1, 0, 0}, {-0.5f, 0.5f,-0.5f}, {0,1}}
			};

			uint32_t cubeIndices[36];
			for (uint32_t face = 0; face < 6; ++face) {
				uint32_t b = face * 4, i = face * 6;
				cubeIndices[i + 0] = b + 0;
				cubeIndices[i + 1] = b + 1;
				cubeIndices[i + 2] = b + 2;
				cubeIndices[i + 3] = b + 2;
				cubeIndices[i + 4] = b + 3;
				cubeIndices[i + 5] = b + 0;
			}

			m_Data.CubeVertexArray = m_RendererAPI.CreateVertexArray();
			Shared<VertexBuffer> cubeVertexBuffer = m_RendererAPI.CreateVertexBuffer(cubeVertices, sizeof(cubeVertices));
			cubeVertexBuffer->SetLayout({
				{"a_Color",    ShaderData::Float4},
				{"a_Normal",   ShaderData::Float3},
				{"a_Position", ShaderData::Float3},
				{"a_TexCoord", ShaderData::Float2}
			});
			m_Data.CubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
			m_Data.CubeVertexArray->SetIndexBuffer(m_RendererAPI.CreateIndexBuffer(cubeIndices, 36));

			// --- Shared Resources ---
			m_Data.CameraUniformBuffer = m_RendererAPI.CreateUniformBuffer(sizeof(CameraData), 0);

			if (m_RendererAPI.TryGetOrLoadShader("assets/shaders/Texture.glsl", m_Data.TextureShader)) {
				m_Data.TextureShader->Bind();
				m_Data.TextureShader->SetInt("u_Texture", 0);
				m_Data.TextureShader->SetInt("u_ShadowMap", 1);
			} else {
				Log::CoreCritical("Failed to load texture shader.");
				throw std::runtime_error("Failed to load texture shader.");
			}

			if (m_RendererAPI.TryGetOrLoadShader("assets/shaders/ShadowDepth.glsl", m_Data.ShadowDepthShader)) {
				m_RendererAPI.CreateShadowMap(2048);
			} else {
				Log::CoreCritical("Failed to load shadow depth shader.");
				throw std::runtime_error("Failed to load shadow depth shader.");
			}
		} else if constexpr (g_API == API::Vulkan) {
			Log::CoreCritical("Vulkan Renderer not implemented yet.");
			throw std::runtime_error("Vulkan Renderer not implemented yet.");
		} else {
			Log::CoreCritical("Unknown rendering API.");
			throw std::runtime_error("Unknown rendering API.");
		}

		m_Data.LightUniformBuffer = m_RendererAPI.CreateUniformBuffer(sizeof(LightData), 1);
	}

	void Renderer::DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) {
		if (GetQuadVertexCount() >= MAX_QUAD_VERTEX_BUFFER_COUNT || m_Data.CurrentTexture != m_Data.WhiteTexture) {
			FlushQuads();
		}

		m_Data.CurrentTexture = m_Data.WhiteTexture;
		CreateQuadVertices(position, size, color);
	}

	void Renderer::DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<Texture2D>& texture, const Math::vec4& tint) {
		if (GetQuadVertexCount() >= MAX_QUAD_VERTEX_BUFFER_COUNT || m_Data.CurrentTexture != texture) {
			FlushQuads();
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

	void Renderer::DrawCube(
		const Math::vec3& position,
		const Math::quat& rotation,
		const Math::vec3& scale,
		const Math::vec4& color
	) const {
		DrawCube(position, rotation, scale, m_Data.WhiteTexture, color);
	}

	void Renderer::DrawCube(
		const Math::vec3& position,
		const Math::quat& rotation,
		const Math::vec3& scale,
		const Shared<Texture2D>& texture,
		const Math::vec4& tint
	) const {
		Math::mat4 modelTransform = Math::translate(Math::mat4(1.0f), position)
			* Math::toMat4(rotation)
			* Math::scale(Math::mat4(1.0f), scale);

		m_Data.TextureShader->Bind();
		m_Data.TextureShader->SetMat4("u_Model", modelTransform);
		m_Data.TextureShader->SetFloat4("u_Tint", tint);
		texture->Bind();
		m_Data.CubeVertexArray->Bind();
		m_RendererAPI.DrawIndexed(m_Data.CubeVertexArray);
	}

	void Renderer::BeginScene(const Math::mat4& projectionView, const Math::vec3& cameraPosition) {
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
		m_MeshBatches.clear();

		m_Data.CameraBufferData.ProjectionView = projectionView;
		m_Data.CameraBufferData.CameraPosition = { cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0f };
		m_Data.CameraUniformBuffer->SetData(&m_Data.CameraBufferData, sizeof(CameraData));
	}

	void Renderer::EndScene() {
		// Shadow pass - render depth from light's perspective
		ShadowPass();

		// Restore viewport and bind shadow texture for sampling
		m_RendererAPI.SetViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

		// Flush 2D Quads
		FlushQuads();

		// Flush 3D Meshes - one draw call per unique mesh, per transform
		// ER TODO: Replace with glDrawElementsInstanced when RendererAPI supports it.
		//				For now, issue one DrawIndexed per transform to get correct results.
		for (auto& batch : m_MeshBatches | std::views::values) {
			for (const auto& instance : batch.Instances) {
				m_Data.TextureShader->Bind();
				m_Data.TextureShader->SetMat4("u_Model", instance.Transform);
				m_Data.TextureShader->SetFloat4("u_Tint", instance.Tint);
				m_Data.TextureShader->SetFloat("u_Metallic", instance.Metallic);
				m_Data.TextureShader->SetFloat("u_Roughness", instance.Roughness);
				m_Data.TextureShader->SetMat4("u_LightSpaceMatrix", m_Data.LightSpaceMatrix);
				m_Data.WhiteTexture->Bind(); // slot 0: albedo
				m_RendererAPI.BindShadowTexture(1); // slot 1: shadow map
				batch.VertexArray->Bind();
				m_RendererAPI.DrawIndexed(batch.VertexArray);
			}
		}
	}

	void Renderer::ShadowPass() {
		m_RendererAPI.BindShadowFrameBuffer();

		// Cull front faces to reduce shadow acne
		// (We'd need glCullFace(GL_FRONT) here - but let's keep it simple
		// and rely on the bias in the shader instead for now)

		for (auto& batch : m_MeshBatches | std::views::values)
		for (const auto& instance : batch.Instances) {
			m_Data.ShadowDepthShader->Bind();
			m_Data.ShadowDepthShader->SetMat4("u_LightSpaceMatrix", m_Data.LightSpaceMatrix);
			m_Data.ShadowDepthShader->SetMat4("u_Model", instance.Transform);
			batch.VertexArray->Bind();
			m_RendererAPI.DrawIndexed(batch.VertexArray);
		}

		m_RendererAPI.UnbindShadowFrameBuffer();
	}

	void Renderer::CreateQuadVertices(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) {
		QuadVertex& vertex0 = m_QuadVertexBufferPtr[0];
		QuadVertex& vertex1 = m_QuadVertexBufferPtr[1];
		QuadVertex& vertex2 = m_QuadVertexBufferPtr[2];
		QuadVertex& vertex3 = m_QuadVertexBufferPtr[3];

		float_t xHalfLength = size.x * 0.5f;
		float_t yHalfLength = size.y * 0.5f;
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

	void Renderer::FlushQuads() {
		if (GetQuadVertexCount() == 0) { return; }

		uint32_t dataSize = GetQuadVertexCount() * sizeof(QuadVertex);
		m_Data.QuadVertexArray->GetVertexBuffers().front()->SetData(m_QuadVertexBufferBase, dataSize);

		if (m_Data.CurrentTexture) { m_Data.CurrentTexture->Bind(); }
		else								{ m_Data.WhiteTexture->Bind(); }

		m_RendererAPI.DrawIndexed(m_Data.QuadVertexArray, GetQuadIndexCount());
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
	}

	void Renderer::SubmitMesh(
		const Shared<VertexArray>& vertexArray,
		const Math::mat4& transform,
		const Math::vec4& tint,
		float_t metallic,
		float_t roughness
	) {
		VertexArray* key = vertexArray.get();
		auto& batch = m_MeshBatches[key];
		if (!batch.VertexArray) { batch.VertexArray = vertexArray; }
		batch.Instances.push_back({ transform, tint, metallic, roughness });
	}

	void Renderer::SetDirectionalLight(
		const Math::vec3& direction,
		const Math::vec3& color,
		float_t colorIntensity,
		const Math::vec3& ambient,
		float_t ambientIntensity
	) {
		Math::vec3 dir = Math::normalize(direction);
		m_Data.LightBufferData.DirectionalLight.Direction = { dir.x, dir.y, dir.z, 0.f };
		m_Data.LightBufferData.DirectionalLight.Color = { color.x, color.y, color.z, colorIntensity };
		m_Data.LightBufferData.DirectionalLight.Ambient = { ambient.x, ambient.y, ambient.z, ambientIntensity };
		m_Data.LightUniformBuffer->SetData(&m_Data.LightBufferData, sizeof(LightData));

		// Compute light-space matrix for shadow mapping
		// Ortho projection from the light's pov
		float_t shadowExtent = 15.0f; // how far the shadow frustum extends
		Math::mat4 lightProj = Math::ortho(
			-shadowExtent, shadowExtent,
			-shadowExtent, shadowExtent,
			0.1f, 50.0f
		);
		// Position the light camera "behind" the light direction, looking at the origin
		Math::vec3 lightPos = -dir * 20.0f; // push back along the opposite of light dir
		Math::mat4 lightView = Math::lookAt(lightPos, Math::vec3(0.0f), Math::vec3(0.0f, 1.0f, 0.0f));

		m_Data.LightSpaceMatrix = lightProj * lightView;
	}

	void Renderer::SetPointLight(uint32_t index, const Math::vec3& position, float_t radius, const Math::vec3& color, float_t colorIntensity) {
		if (index >= MAX_POINT_LIGHTS) {
			Log::CoreWarn("Point light index {} exceeds maximum ({}).", index, MAX_POINT_LIGHTS);
			return;
		}

		auto& pointLight = m_Data.LightBufferData.PointLights[index];
		pointLight.Position = { position.x, position.y, position.z, radius };
		pointLight.Color = { color.x, color.y, color.z, colorIntensity };
		m_Data.LightUniformBuffer->SetData(&m_Data.LightBufferData, sizeof(LightData));
	}

	void Renderer::SetPointLightCount(uint32_t count) {
		m_Data.LightBufferData.PointLightCount.x = static_cast<float_t>(count);
		m_Data.LightUniformBuffer->SetData(&m_Data.LightBufferData, sizeof(LightData));
	}

	void Renderer::SetViewportSize(uint32_t width, uint32_t height) {
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}
}
