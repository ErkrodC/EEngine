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

namespace EEngine::Rendering {
	constexpr uint32_t MAX_QUAD_VERTEX_BUFFER_COUNT = MAX_QUADS * 4;
	constexpr uint32_t MAX_QUAD_VERTEX_BUFFER_SIZE = MAX_QUAD_VERTEX_BUFFER_COUNT * sizeof(QuadVertex);
	constexpr uint32_t MAX_QUAD_INDEX_BUFFER_COUNT = MAX_QUADS * 6;

	export class Renderer {
	public:
		explicit Renderer(RendererAPI& rendererAPI);

		void BeginScene(const Math::mat4& projectionView = Math::mat4(1.0f));
		void EndScene(); // Execution phase: Flush 2D and 3D batches

		// 2D Path: Batching
		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color);
		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<Texture2D>& texture, const Math::vec4& tint = Math::vec4(1.0f));
		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color);
		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Shared<Texture2D>& texture);
		void DrawCube(const Math::vec3& position, const Math::quat& rotation, const Math::vec3& scale, const Shared<Texture2D>& texture) const;
		void FlushQuads();

		// 3D Path: Instancing (submit during scene traversal, draw in EndScene)
		void SubmitMesh(const Shared<VertexArray>& vertexArray, const Math::mat4& transform);

	private:
		RendererAPI& m_RendererAPI;
		struct RendererData {
			Shared<Shader> TextureShader;
			Shared<Texture2D> WhiteTexture;
			Shared<VertexArray> QuadVertexArray;
			Shared<Texture2D> CurrentTexture;

			struct CameraData {
				Math::mat4 ProjectionView;
			} CameraBufferData;
			Shared<UniformBuffer> CameraUniformBuffer;
		} m_Data;
		QuadVertex m_QuadVertexBufferBase[MAX_QUAD_VERTEX_BUFFER_COUNT]{};
		QuadVertex* m_QuadVertexBufferPtr;
		uint32_t m_QuadIndexBufferPtr[MAX_QUAD_INDEX_BUFFER_COUNT];

		struct InstancedBatch {
			Shared<VertexArray> VertexArray;
			std::vector<Math::mat4> Transforms;
		};

		// Key = raw VertexArray pointer for identity comparison, (ownership held by InstancedBatch::VertexArray)
		std::unordered_map<VertexArray*, InstancedBatch> m_MeshBatches;

		void CreateQuadVertices(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color);
		uint32_t GetQuadVertexCount() const { return m_QuadVertexBufferPtr - m_QuadVertexBufferBase; }
		uint32_t GetQuadIndexCount() const { return GetQuadVertexCount() / 4 * 6; }
	};
}
