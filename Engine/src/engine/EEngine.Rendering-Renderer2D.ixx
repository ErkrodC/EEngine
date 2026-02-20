module;
// GLM operators require direct include in global module fragment for ADL to work with templates
#include <glm/detail/type_mat4x4.hpp>

export module EEngine.Rendering:Renderer2D;
import EEngine.Core;
import EEngine.Math;
import :Buffers;
import :Camera;
import :Interfaces;
import :Renderer;

using namespace EEngine;

export namespace EEngine::Renderer2D {
	// ============================================================================
	// 2D Renderer
	// ============================================================================

	struct Renderer2DData {
		Shared<IVertexArray> QuadVertexArray{};
		Shared<IShader> TextureShader{};
		Shared<ITexture2D> WhiteTexture{};
	} *s_Data ;

	void Initialize() {
		s_Data = new Renderer2DData();
		s_Data->QuadVertexArray = RendererAPI::CreateVertexArray();
		s_Data->WhiteTexture = RendererAPI::CreateTexture2D(1, 1);

		float vertices[4*5] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,
		};

		Shared<IVertexBuffer> vertexBuffer = RendererAPI::CreateVertexBuffer(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ ShaderData::Float3, "a_Position" },
			{ ShaderData::Float2, "v_TexCoord" }
		});
		s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Shared<IIndexBuffer> indexBuffer = RendererAPI::CreateIndexBuffer(indices, sizeof(indices)/sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_Data->TextureShader = Renderer::GetShaderLibrary()->Load("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Shutdown() {
		delete s_Data;
	}

	void BeginScene(const Camera& camera) {
		auto textureShader = s_Data->TextureShader;
		textureShader->Bind();
		textureShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
	}

	void EndScene() {

	}

	void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) {
		auto shader = s_Data->TextureShader;
		shader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		Math::mat4 pos = Math::translate(Math::Identity::mat4, position);
		Math::mat4 rot = Math::Identity::mat4; // ER TODO rotation
		Math::mat4 scale = Math::scale(Math::Identity::mat4, { size.x, size.y, 1.0f });
		Math::mat4 transform = pos * rot * scale;
		shader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererAPI::DrawIndexed(s_Data->QuadVertexArray);
	}

	void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) {
		DrawQuad({position.x, position.y, 0.0f }, size, color);
	}

	void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<ITexture2D>& texture) {
		auto shader = s_Data->TextureShader;
		shader->SetFloat4("u_Color", Math::vec4(1.0f));
		texture->Bind();

		Math::mat4 pos = Math::translate(Math::Identity::mat4, position);
		Math::mat4 rot = Math::Identity::mat4; // ER TODO rotation
		Math::mat4 scale = Math::scale(Math::Identity::mat4, { size.x, size.y, 1.0f });
		Math::mat4 transform = pos * rot * scale;
		shader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererAPI::DrawIndexed(s_Data->QuadVertexArray);
	}

	void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Shared<ITexture2D>& texture) {
		DrawQuad({position.x, position.y, 0.0f }, size, texture);
	}
}
