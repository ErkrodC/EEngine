module;
#include <Profiling/Profiling.hpp>

export module Runner:RunnerLayer3D;
import EEngine;
import EEngine.Profiling;

using namespace EEngine;

export class RunnerLayer3D : public Layer {
public:
	RunnerLayer3D()
		: Layer("RunnerLayer3D")
		, m_CameraController(16.0f / 9.0f) {
		m_VertexArray = RendererAPI::CreateVertexArray();

		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,		0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.8f, 1.0f,		1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.8f, 0.8f, 0.8f, 1.0f,		0.0f, 1.0f,
		};

		Shared<VertexBuffer> vertexBuffer;
		vertexBuffer = RendererAPI::CreateVertexBuffer(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{ ShaderData::Float3, "a_Position" },
			{ ShaderData::Float4, "a_Color" },
			{ ShaderData::Float2, "a_TexCoord" },
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Shared<IndexBuffer> indexBuffer;
		indexBuffer = RendererAPI::CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		auto textureShader = Renderer::GetShaderLibrary()->Load("assets/shaders/Texture.glsl");
		m_Texture = RendererAPI::CreateTexture2D("assets/textures/test.png");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	}

	void OnUpdate(Timestep timestep) override {
		EE_PROFILE_FUNCTION();

		RendererAPI::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		m_CameraController.OnUpdate(timestep);
		HandleTriMovement(timestep);

		Renderer::BeginScene(m_CameraController.GetCamera()); {
			m_Texture->Bind();
			if (Shared<Shader> textureShader; Renderer::GetShaderLibrary()->TryGet("Texture", textureShader)) {
				Renderer::Submit(textureShader, m_VertexArray, Math::translate(Math::mat4(1.0f), m_TriPos));
			}
		} Renderer::EndScene();
	};

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
	Shared<VertexArray> m_VertexArray;
	Shared<Texture2D> m_Texture;
	CameraController m_CameraController;
	Math::vec3 m_TriPos{};
};
