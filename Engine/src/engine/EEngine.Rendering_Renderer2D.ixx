module;
// GLM operators require direct include in global module fragment for ADL to work with templates
#include <glm/detail/type_mat4x4.hpp>

export module EEngine.Rendering:Renderer2D;
import EEngine.Core;
import EEngine.Math;
import :Camera;
import :Interfaces;
import :Renderer;

using namespace EEngine;

namespace EEngine::Renderer2D {
	using namespace RendererAPI;

	inline static struct Renderer2DData {
		Shared<VertexArray> QuadVertexArray;
		Shared<Shader> TextureShader;
		Shared<Texture2D> WhiteTexture;
	} s_Data;

	export {
		void Initialize() {
			using namespace RendererAPI;

			if constexpr (g_API == API::DirectX) {
				Log::CoreCritical("DirectX Renderer2D not implemented yet.");
				throw std::runtime_error("DirectX Renderer2D not implemented yet.");
			} else if constexpr (g_API == API::OpenGL) {
				s_Data.QuadVertexArray = CreateVertexArray();
				s_Data.WhiteTexture = CreateTexture2D(1, 1);

				float vertices[4*5] = {
					-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
					0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
					0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
					-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,
				};

				Shared<VertexBuffer> vertexBuffer = CreateVertexBuffer(vertices, sizeof(vertices));
				vertexBuffer->SetLayout({
					{ ShaderData::Float3, "a_Position" },
					{ ShaderData::Float2, "v_TexCoord" }
				});
				s_Data.QuadVertexArray->AddVertexBuffer(vertexBuffer);

				uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
				Shared<IndexBuffer> indexBuffer = CreateIndexBuffer(indices, sizeof(indices)/sizeof(uint32_t));
				s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);

				s_Data.TextureShader = Renderer::GetShaderLibrary()->Load("assets/shaders/Texture.glsl");
				s_Data.TextureShader->Bind();
				s_Data.TextureShader->SetInt("u_Texture", 0);
			} else if constexpr (g_API == API::Vulkan) {
				Log::CoreCritical("Vulkan Renderer2D not implemented yet.");
				throw std::runtime_error("Vulkan Renderer2D not implemented yet.");
			} else {
				Log::CoreCritical("Unknown rendering API.");
				throw std::runtime_error("Unknown rendering API.");
			}
		}

		void BeginScene(const Camera& camera) {
			s_Data.TextureShader->Bind();
			s_Data.TextureShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
		}

		void EndScene() {}

		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) {
			using namespace Math;
			using namespace RendererAPI;

			if constexpr (g_API == API::DirectX) {
				Log::CoreCritical("DirectX Renderer2D not implemented yet.");
				throw std::runtime_error("DirectX Renderer2D not implemented yet.");
			} else if constexpr (g_API == API::OpenGL) {
				s_Data.TextureShader->SetFloat4("u_Color", color);
				s_Data.WhiteTexture->Bind();

				mat4 pos = translate(Identity::mat4, position);
				mat4 rot = Identity::mat4; // ER TODO rotation
				mat4 scale = Math::scale(Identity::mat4, {size.x, size.y, 1.0f});
				mat4 transform = pos * rot * scale;
				s_Data.TextureShader->SetMat4("u_Transform", transform);

				s_Data.QuadVertexArray->Bind();
				DrawIndexed(s_Data.QuadVertexArray);
			} else if constexpr (g_API == API::Vulkan) {
				Log::CoreCritical("Vulkan Renderer2D not implemented yet.");
				throw std::runtime_error("Vulkan Renderer2D not implemented yet.");
			} else {
				Log::CoreCritical("Unknown rendering API.");
				throw std::runtime_error("Unknown rendering API.");
			}
		}

		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) {
			DrawQuad({position.x, position.y, 0.0f}, size, color);
		}

		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<Texture2D>& texture) {
			using namespace Math;
			using namespace RendererAPI;

			if constexpr (g_API == API::DirectX) {
				Log::CoreCritical("DirectX Renderer2D not implemented yet.");
				throw std::runtime_error("DirectX Renderer2D not implemented yet.");
			} else if constexpr (g_API == API::OpenGL) {
				s_Data.TextureShader->SetFloat4("u_Color", vec4(1.0f));
				texture->Bind();

				mat4 pos = translate(Identity::mat4, position);
				mat4 rot = Identity::mat4; // ER TODO rotation
				mat4 scale = Math::scale(Identity::mat4, {size.x, size.y, 1.0f});
				mat4 transform = pos * rot * scale;
				s_Data.TextureShader->SetMat4("u_Transform", transform);

				s_Data.QuadVertexArray->Bind();
				DrawIndexed(s_Data.QuadVertexArray);
			} else if constexpr (g_API == API::Vulkan) {
				Log::CoreCritical("Vulkan Renderer2D not implemented yet.");
				throw std::runtime_error("Vulkan Renderer2D not implemented yet.");
			} else {
				Log::CoreCritical("Unknown rendering API.");
				throw std::runtime_error("Unknown rendering API.");
			}
		}

		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Shared<Texture2D>& texture) {
			DrawQuad({position.x, position.y, 0.0f}, size, texture);
		}
	}
}
