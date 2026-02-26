module;
// GLM operators require direct include in global module fragment for ADL to work with templates
#include <glm/detail/type_mat4x4.hpp>

export module EEngine.Rendering:Renderer;
import EEngine.Core;
import EEngine.Math;
import :Camera;
import :API;
import :IndexBuffer;
import :VertexBuffer;
import :VertexArray;
import :Shader;
import :Texture;
import :RendererAPI;

using namespace EEngine;

namespace EEngine::Rendering {
	export class Renderer {
	public:
		explicit Renderer(RendererAPI& rendererAPI) : m_RendererAPI(rendererAPI) {
			if constexpr (g_API == API::DirectX) {
				Log::CoreCritical("DirectX Renderer not implemented yet.");
				throw std::runtime_error("DirectX Renderer not implemented yet.");
			} else if constexpr (g_API == API::OpenGL) {
				m_Data.QuadVertexArray = m_RendererAPI.CreateVertexArray();
				m_Data.WhiteTexture = m_RendererAPI.CreateTexture2D(1, 1);

				float vertices[4 * 5] = {
					-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
					0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
					0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
					-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				};

				Shared<VertexBuffer> vertexBuffer = m_RendererAPI.CreateVertexBuffer(vertices, sizeof(vertices));
				vertexBuffer->SetLayout({
					{ShaderData::Float3, "a_Position"},
					{ShaderData::Float2, "v_TexCoord"}
				});
				m_Data.QuadVertexArray->AddVertexBuffer(vertexBuffer);

				uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
				Shared<IndexBuffer> indexBuffer = m_RendererAPI.CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
				m_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);

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

		void BeginScene(const Math::mat4& projectionView = Math::mat4(1.0f)) const {
			m_Data.TextureShader->Bind();
			m_Data.TextureShader->SetMat4("u_ProjectionView", projectionView);
		}

		void EndScene() const {}

		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) const {
			using namespace Math;

			if constexpr (g_API == API::DirectX) {
				Log::CoreCritical("DirectX Renderer not implemented yet.");
				throw std::runtime_error("DirectX Renderer not implemented yet.");
			} else if constexpr (g_API == API::OpenGL) {
				m_Data.TextureShader->SetFloat4("u_Color", color);
				m_Data.WhiteTexture->Bind();

				mat4 pos = translate(Identity::mat4, position);
				mat4 rot = Identity::mat4; // ER TODO rotation
				mat4 scale = Math::scale(Identity::mat4, {size.x, size.y, 1.0f});
				mat4 transform = pos * rot * scale;
				m_Data.TextureShader->SetMat4("u_Transform", transform);

				m_Data.QuadVertexArray->Bind();
				m_RendererAPI.DrawIndexed(m_Data.QuadVertexArray);
			} else if constexpr (g_API == API::Vulkan) {
				Log::CoreCritical("Vulkan Renderer not implemented yet.");
				throw std::runtime_error("Vulkan Renderer not implemented yet.");
			} else {
				Log::CoreCritical("Unknown rendering API.");
				throw std::runtime_error("Unknown rendering API.");
			}
		}

		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) const {
			DrawQuad({position.x, position.y, 0.0f}, size, color);
		}

		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<Texture2D>& texture) const {
			using namespace Math;

			if constexpr (g_API == API::DirectX) {
				Log::CoreCritical("DirectX Renderer not implemented yet.");
				throw std::runtime_error("DirectX Renderer not implemented yet.");
			} else if constexpr (g_API == API::OpenGL) {
				m_Data.TextureShader->SetFloat4("u_Color", vec4(1.0f));
				texture->Bind();

				mat4 pos = translate(Identity::mat4, position);
				mat4 rot = Identity::mat4; // ER TODO rotation
				mat4 scale = Math::scale(Identity::mat4, {size.x, size.y, 1.0f});
				mat4 transform = pos * rot * scale;
				m_Data.TextureShader->SetMat4("u_Transform", transform);

				m_Data.QuadVertexArray->Bind();
				m_RendererAPI.DrawIndexed(m_Data.QuadVertexArray);
			} else if constexpr (g_API == API::Vulkan) {
				Log::CoreCritical("Vulkan Renderer not implemented yet.");
				throw std::runtime_error("Vulkan Renderer not implemented yet.");
			} else {
				Log::CoreCritical("Unknown rendering API.");
				throw std::runtime_error("Unknown rendering API.");
			}
		}

		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Shared<Texture2D>& texture) const {
			DrawQuad({position.x, position.y, 0.0f}, size, texture);
		}

		void Submit(
			const Shared<Shader>& shader,
			const Shared<VertexArray>& vertexArray,
			const Math::mat4& transform = Math::mat4(1.0f),
			const Math::mat4& projectionView = Math::mat4(1.0f)
		) const {
			shader->Bind();
			shader->SetMat4("u_ProjectionView", projectionView);
			shader->SetMat4("u_Transform", transform);
			vertexArray->Bind();
			m_RendererAPI.DrawIndexed(vertexArray);
		}

	private:
		RendererAPI& m_RendererAPI;
		struct RendererData {
			Shared<Shader> TextureShader;
			Shared<Texture2D> WhiteTexture;
			Shared<VertexArray> QuadVertexArray;
		} m_Data;
	};
}
