module;
#include <Profiling/Profiling.hpp>

export module Runner:RunnerLayer3D;
import EEngine;
import EEngine.Profiling;

using namespace EEngine;
using namespace Rendering;

export class RunnerLayer3D : public Layer {
public:
	RunnerLayer3D(RendererAPI& rendererAPI, Renderer& renderer)
		: Layer("RunnerLayer3D")
		, m_CameraController(16.0f / 9.0f)
		, m_RendererAPI(rendererAPI)
		, m_Renderer(renderer)
	{
		m_VertexArray = m_RendererAPI.CreateVertexArray();

		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,		0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.8f, 1.0f,		1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.8f, 0.8f, 0.8f, 1.0f,		0.0f, 1.0f,
		};

		Shared<VertexBuffer> vertexBuffer;
		vertexBuffer = m_RendererAPI.CreateVertexBuffer(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{ ShaderData::Float3, "a_Position" },
			{ ShaderData::Float4, "a_Color" },
			{ ShaderData::Float2, "a_TexCoord" },
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Shared<IndexBuffer> indexBuffer;
		indexBuffer = m_RendererAPI.CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		Shared<Shader> textureShader;
		if (m_RendererAPI.TryGetOrLoadShader("assets/shaders/Texture.glsl", textureShader)) {
			m_Texture = m_RendererAPI.CreateTexture2D("assets/textures/test.png");

			textureShader->Bind();
			textureShader->SetInt("u_Texture", 0);
		} else {
			Log::CoreCritical("Failed to load texture shader.");
			throw std::runtime_error("Failed to load texture shader.");
		}
	}

	void OnUpdate(Timestep timestep) override {
		EE_PROFILE_FUNCTION();

		m_RendererAPI.Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		m_CameraController.OnUpdate(timestep);
		HandleTriMovement(timestep);

		Math::mat4 projectionView = m_CameraController.GetCamera().GetProjectionViewMatrix();
		m_Renderer.BeginScene(projectionView);
		m_Texture->Bind();

		Shared<Shader> textureShader;
		if (m_RendererAPI.TryGetOrLoadShader("Texture", textureShader)) {
			m_Renderer.Submit(
				textureShader,
				m_VertexArray,
				Math::translate(Math::mat4(1.0f), m_TriPos),
				projectionView
			);
		} else {
			Log::CoreCritical("Failed to load texture shader.");
			throw std::runtime_error("Failed to load texture shader.");
		}
		m_Renderer.EndScene();
	}

	void OnIMGUIRender() override {

	}

	void OnEvent(Event& event) override {
		m_CameraController.OnEvent(event);
	}

	void HandleTriMovement(Timestep timestep) {
		float deltaDist = 1.0f * timestep.GetSeconds();

		if (Input::IsKeyPressed(KeyCode::I)) {
			m_TriPos.y += deltaDist;
		}

		if (Input::IsKeyPressed(KeyCode::J)) {
			m_TriPos.x -= deltaDist;
		}

		if (Input::IsKeyPressed(KeyCode::K)) {
			m_TriPos.y -= deltaDist;
		}

		if (Input::IsKeyPressed(KeyCode::L)) {
			m_TriPos.x += deltaDist;
		}
	}

private:
	CameraController m_CameraController;
	RendererAPI& m_RendererAPI;
	Renderer& m_Renderer;
	Shared<VertexArray> m_VertexArray;
	Shared<Texture2D> m_Texture;
	Math::vec3 m_TriPos{};
};
