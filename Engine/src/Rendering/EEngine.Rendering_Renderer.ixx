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
	export constexpr uint32_t MAX_POINT_LIGHTS = 4;
	constexpr uint32_t MAX_QUAD_VERTEX_BUFFER_COUNT = MAX_QUADS * 4;
	constexpr uint32_t MAX_QUAD_VERTEX_BUFFER_SIZE = MAX_QUAD_VERTEX_BUFFER_COUNT * sizeof(QuadVertex);
	constexpr uint32_t MAX_QUAD_INDEX_BUFFER_COUNT = MAX_QUADS * 6;

	struct PointLight {
		Math::vec4 Position; // XYZ, W = radius
		Math::vec4 Color; // RGB, A = intensity
	};

	struct DirectionalLight {
		Math::vec4 Direction; // XYZ, w = unused
		Math::vec4 Color; // RGB, A = intensity
		Math::vec4 Ambient; // RGB, A = strength
	};

	struct LightData {
		DirectionalLight DirectionalLight;
		PointLight PointLights[MAX_POINT_LIGHTS];
		Math::vec4 PointLightCount; // Active Count, yzw = unused
	};

	struct CameraData {
		Math::mat4 ProjectionView;
		Math::vec4 CameraPosition;
	};

	struct RendererData {
		Shared<Shader> TextureShader;
		Shared<Shader> ShadowDepthShader;
		Shared<Shader> ToneMapShader;
		Shared<Texture2D> WhiteTexture;
		Shared<VertexArray> QuadVertexArray;
		Shared<VertexArray> CubeVertexArray;
		Shared<Texture2D> CurrentTexture;

		CameraData CameraBufferData;
		Shared<UniformBuffer> CameraUniformBuffer;

		LightData LightBufferData;
		Shared<UniformBuffer> LightUniformBuffer;

		Math::mat4 LightSpaceMatrix = Math::mat4(1.0f);

		uint32_t FullscreenVAO = 0; // empty VAO for fullscreen triangle
		float_t Exposure = 1.0f;
	};

	struct Instance {
		Math::mat4 Transform;
		Math::vec4 Tint;
		float_t Metallic = 0.0f;
		float_t Roughness = 0.5f;
	};

	struct InstancedBatch {
		Shared<VertexArray> VertexArray;
		std::vector<Instance> Instances;
	};

	export class Renderer {
	public:
		explicit Renderer(RendererAPI& rendererAPI);

		void BeginScene(const Math::mat4& projectionView = Math::mat4(1.0f), const Math::vec3& cameraPosition = Math::vec3(0.0f));
		void EndScene(); // Execution phase: Flush 2D and 3D batches

		// 2D Path: Batching
		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color);
		void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Shared<Texture2D>& texture, const Math::vec4& tint = Math::vec4(1.0f));
		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color);
		void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Shared<Texture2D>& texture);
		void FlushQuads();

		// 3D Path: Instancing (submit during scene traversal, draw in EndScene)
		void DrawCube(const Math::vec3& position, const Math::quat& rotation, const Math::vec3& scale, const Math::vec4& color) const;
		void DrawCube(const Math::vec3& position, const Math::quat& rotation, const Math::vec3& scale, const Shared<Texture2D>& texture, const Math::vec4& tint) const;
		void SubmitMesh(
			const Shared<VertexArray>& vertexArray,
			const Math::mat4& transform,
			const Math::vec4& tint = Math::vec4(1.0f),
			float_t metallic = 0.0f,
			float_t roughness = 0.5f
		);

		void SetDirectionalLight(const Math::vec3& direction, const Math::vec3& color, float_t colorIntensity, const Math::vec3& ambient, float_t ambientIntensity);
		void SetPointLight(uint32_t index, const Math::vec3& position, float_t radius, const Math::vec3& color, float_t colorIntensity);
		void SetPointLightCount(uint32_t count);

		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		RendererAPI& m_RendererAPI;
		RendererData m_Data;
		uint32_t m_ViewportWidth = 1280, m_ViewportHeight = 720;
		QuadVertex m_QuadVertexBufferBase[MAX_QUAD_VERTEX_BUFFER_COUNT]{};
		QuadVertex* m_QuadVertexBufferPtr;
		uint32_t m_QuadIndexBufferPtr[MAX_QUAD_INDEX_BUFFER_COUNT];

		// Key = raw VertexArray pointer for identity comparison, (ownership held by InstancedBatch::VertexArray)
		std::unordered_map<VertexArray*, InstancedBatch> m_MeshBatches;

		void ShadowPass();
		void CreateQuadVertices(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color);
		uint32_t GetQuadVertexCount() const { return m_QuadVertexBufferPtr - m_QuadVertexBufferBase; }
		uint32_t GetQuadIndexCount() const { return GetQuadVertexCount() / 4 * 6; }
	};
}
