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

export namespace EEngine::Rendering {
	class Renderer {
	public:
		explicit Renderer(RendererAPI& rendererAPI);

		void BeginScene(const Math::mat4& projectionView = Math::mat4(1.0f)) const;
		void EndScene() const;

		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) const;
		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) const;
		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<Texture2D>& texture) const;
		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Shared<Texture2D>& texture) const;

		void Submit(
			const Shared<Shader>& shader,
			const Shared<VertexArray>& vertexArray,
			const Math::mat4& transform = Math::mat4(1.0f),
			const Math::mat4& projectionView = Math::mat4(1.0f)
		) const;

	private:
		RendererAPI& m_RendererAPI;
		struct RendererData {
			Shared<Shader> TextureShader;
			Shared<Texture2D> WhiteTexture;
			Shared<VertexArray> QuadVertexArray;
		} m_Data;
	};
}
