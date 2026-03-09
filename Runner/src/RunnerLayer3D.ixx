module;
#include <Profiling/Profiling.hpp>

export module Runner:RunnerLayer3D;
import EEngine;
import EEngine.Profiling;

using namespace EEngine;
using namespace Rendering;

// ============================================================================
// Mesh Builders (temporary helpers until asset loading exists)
// ============================================================================
namespace MeshBuilder {
	struct MeshData {
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
	};

	// Unit cube centered at origin, with outward-facing normals
	inline MeshData Cube() {
		MeshData m;
		Math::vec4 white = { 1.0f, 1.0f, 1.0f, 1.0f };

		// Front face (z = +0.5)
		m.Vertices.push_back({ white, { 0, 0, 1}, {-0.5f, -0.5f,  0.5f}, {0, 0} });
		m.Vertices.push_back({ white, { 0, 0, 1}, { 0.5f, -0.5f,  0.5f}, {1, 0} });
		m.Vertices.push_back({ white, { 0, 0, 1}, { 0.5f,  0.5f,  0.5f}, {1, 1} });
		m.Vertices.push_back({ white, { 0, 0, 1}, {-0.5f,  0.5f,  0.5f}, {0, 1} });
		// Back face (z = -0.5)
		m.Vertices.push_back({ white, { 0, 0,-1}, { 0.5f, -0.5f, -0.5f}, {0, 0} });
		m.Vertices.push_back({ white, { 0, 0,-1}, {-0.5f, -0.5f, -0.5f}, {1, 0} });
		m.Vertices.push_back({ white, { 0, 0,-1}, {-0.5f,  0.5f, -0.5f}, {1, 1} });
		m.Vertices.push_back({ white, { 0, 0,-1}, { 0.5f,  0.5f, -0.5f}, {0, 1} });
		// Top face (y = +0.5)
		m.Vertices.push_back({ white, { 0, 1, 0}, {-0.5f,  0.5f,  0.5f}, {0, 0} });
		m.Vertices.push_back({ white, { 0, 1, 0}, { 0.5f,  0.5f,  0.5f}, {1, 0} });
		m.Vertices.push_back({ white, { 0, 1, 0}, { 0.5f,  0.5f, -0.5f}, {1, 1} });
		m.Vertices.push_back({ white, { 0, 1, 0}, {-0.5f,  0.5f, -0.5f}, {0, 1} });
		// Bottom face (y = -0.5)
		m.Vertices.push_back({ white, { 0,-1, 0}, {-0.5f, -0.5f, -0.5f}, {0, 0} });
		m.Vertices.push_back({ white, { 0,-1, 0}, { 0.5f, -0.5f, -0.5f}, {1, 0} });
		m.Vertices.push_back({ white, { 0,-1, 0}, { 0.5f, -0.5f,  0.5f}, {1, 1} });
		m.Vertices.push_back({ white, { 0,-1, 0}, {-0.5f, -0.5f,  0.5f}, {0, 1} });
		// Right face (x = +0.5)
		m.Vertices.push_back({ white, { 1, 0, 0}, { 0.5f, -0.5f,  0.5f}, {0, 0} });
		m.Vertices.push_back({ white, { 1, 0, 0}, { 0.5f, -0.5f, -0.5f}, {1, 0} });
		m.Vertices.push_back({ white, { 1, 0, 0}, { 0.5f,  0.5f, -0.5f}, {1, 1} });
		m.Vertices.push_back({ white, { 1, 0, 0}, { 0.5f,  0.5f,  0.5f}, {0, 1} });
		// Left face (x = -0.5)
		m.Vertices.push_back({ white, {-1, 0, 0}, {-0.5f, -0.5f, -0.5f}, {0, 0} });
		m.Vertices.push_back({ white, {-1, 0, 0}, {-0.5f, -0.5f,  0.5f}, {1, 0} });
		m.Vertices.push_back({ white, {-1, 0, 0}, {-0.5f,  0.5f,  0.5f}, {1, 1} });
		m.Vertices.push_back({ white, {-1, 0, 0}, {-0.5f,  0.5f, -0.5f}, {0, 1} });

		for (uint32_t face = 0; face < 6; ++face) {
			uint32_t base = face * 4;
			m.Indices.insert(m.Indices.end(), {
				base + 0, base + 1, base + 2,
				base + 2, base + 3, base + 0
			});
		}
		return m;
	}

	// Flat quad on the XZ plane (y = 0), normal pointing up — useful as a floor
	inline MeshData Plane(float_t size = 5.0f) {
		MeshData m;
		Math::vec4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
		float_t h = size * 0.5f;

		m.Vertices.push_back({ white, {0, 1, 0}, {-h, 0, -h}, {0, 0} });
		m.Vertices.push_back({ white, {0, 1, 0}, { h, 0, -h}, {1, 0} });
		m.Vertices.push_back({ white, {0, 1, 0}, { h, 0,  h}, {1, 1} });
		m.Vertices.push_back({ white, {0, 1, 0}, {-h, 0,  h}, {0, 1} });

		m.Indices = { 0, 1, 2, 2, 3, 0 };
		return m;
	}
}

// ============================================================================
// Helper: build a VertexArray from MeshData
// ============================================================================
Shared<VertexArray> BuildVAO(RendererAPI& api, const MeshBuilder::MeshData& data) {
	Shared<VertexArray> vao = api.CreateVertexArray();

	Shared<VertexBuffer> vbo = api.CreateVertexBuffer(
		const_cast<Vertex*>(data.Vertices.data()),
		static_cast<uint32_t>(data.Vertices.size() * sizeof(Vertex))
	);
	vbo->SetLayout({
		{ "a_Color",    ShaderData::Float4 },
		{ "a_Normal",   ShaderData::Float3 },
		{ "a_Position", ShaderData::Float3 },
		{ "a_TexCoord", ShaderData::Float2 },
	});
	vao->AddVertexBuffer(vbo);

	Shared<IndexBuffer> ibo = api.CreateIndexBuffer(
		const_cast<uint32_t*>(data.Indices.data()),
		static_cast<uint32_t>(data.Indices.size())
	);
	vao->SetIndexBuffer(ibo);

	return vao;
}

// ============================================================================
// RunnerLayer3D
// ============================================================================
export class RunnerLayer3D : public Layer {
public:
	RunnerLayer3D(RendererAPI& rendererAPI, Renderer& renderer, Input& input)
		: Layer("RunnerLayer3D")
		, m_Input(input)
		, m_RendererAPI(rendererAPI)
		, m_Renderer(renderer)
		, m_Scene(MakeUnique<Scene>())
	{
		Shared<Shader> textureShader;
		if (!m_RendererAPI.TryGetOrLoadShader("assets/shaders/Texture.glsl", textureShader)) {
			Log::CoreCritical("Failed to load texture shader.");
			throw std::runtime_error("Failed to load texture shader.");
		}

		// --- Register meshes ---
		uint32_t cubeMeshID = m_Scene->RegisterMesh(BuildVAO(m_RendererAPI, MeshBuilder::Cube()));
		uint32_t groundMeshID = m_Scene->RegisterMesh(BuildVAO(m_RendererAPI, MeshBuilder::Plane(10.0f)));

		// --- Camera ---
		SceneEntity cameraEntity = m_Scene->CreateEntity("Main Camera");
		auto& cam = cameraEntity.AddComponent<CameraComponent>();
		cam.Projection = Math::perspective(Math::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
		cam.IsPrimary = true;
		// Pull back and up
		auto& camT = cameraEntity.GetComponent<TransformComponent>();
		camT.Transform.Position = { 0.0f, 2.0f, 10.0f };
		// Tilt camera down
		float pitch = Math::radians(-20.0f);
		camT.Transform.Rotation = {pitch, 0.0f, 0.0f};

		// --- Ground plane ---
		SceneEntity ground = m_Scene->CreateEntity("Ground");
		auto& groundMesh = ground.AddComponent<MeshComponent>();
		groundMesh.VertexArrayID = groundMeshID;
		groundMesh.Color = { 0.4f, 0.4f, 0.4f, 1.0f };
		ground.GetComponent<TransformComponent>().Transform.Position = { 0.0f, -1.0f, 0.0f };

		// --- Red cube (left) ---
		SceneEntity redCube = m_Scene->CreateEntity("Red Cube");
		auto& redMesh = redCube.AddComponent<MeshComponent>();
		redMesh.VertexArrayID = cubeMeshID;
		redMesh.Color = { 0.8f, 0.2f, 0.3f, 1.0f };
		redCube.GetComponent<TransformComponent>().Transform.Position = { -2.0f, 0.0f, 0.0f };

		// --- Green cube (center) ---
		SceneEntity greenCube = m_Scene->CreateEntity("Green Cube");
		auto& greenMesh = greenCube.AddComponent<MeshComponent>();
		greenMesh.VertexArrayID = cubeMeshID;
		greenMesh.Color = { 0.2f, 0.8f, 0.3f, 1.0f };
		greenCube.GetComponent<TransformComponent>().Transform.Position = { 0.0f, 0.0f, 0.0f };

		// --- Blue cube (right) ---
		SceneEntity blueCube = m_Scene->CreateEntity("Blue Cube");
		auto& blueMesh = blueCube.AddComponent<MeshComponent>();
		blueMesh.VertexArrayID = cubeMeshID;
		blueMesh.Color = { 0.2f, 0.3f, 0.8f, 1.0f };
		blueCube.GetComponent<TransformComponent>().Transform.Position = { 2.0f, 0.0f, 0.0f };
	}

	void OnUpdate(Timestep timestep) override {
		EE_PROFILE_FUNCTION();

		m_RendererAPI.Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		m_Rotation += static_cast<float>(timestep.GetSeconds());
		auto cubeEntities = m_Scene->View<TransformComponent, MeshComponent>();
		for (auto entityID : cubeEntities) {
			auto& transform = m_Scene->Get<TransformComponent>(entityID);
			// skip the floor (it's at y = -1)
			if (transform.Transform.Position.y < -0.5f) { continue; }
			transform.Transform.Rotation = {
				m_Rotation * 0.5f,
				m_Rotation,
				0.0f
			};
		}

		m_Scene->OnUpdate(timestep);
		m_Scene->OnRender(m_Renderer);
	}

	void OnIMGUIRender() override {}

	void OnEvent(Event& event) override {}

private:
	Input& m_Input;
	RendererAPI& m_RendererAPI;
	Renderer& m_Renderer;
	Unique<Scene> m_Scene;

	float_t m_Rotation = 0.0f;
};