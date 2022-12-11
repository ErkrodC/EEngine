//module;
#include <memory>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "EEngine.hpp"

//export module Runner;
import EEngine.Core;
import EEngine.Application;
import EEngine.Event;
import EEngine.Rendering;
//#include <Platform/OpenGL/OpenGLShader.ixx>

class ExampleLayer : public EEngine::Layer {
public:
	ExampleLayer()
			: Layer("Example")
			, m_CameraController(16.0f / 9.0f) {
		m_VertexArray = EEngine::RendererAPI::CreateVertexArray();

		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,		0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.8f, 1.0f,		1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.8f, 0.8f, 0.8f, 1.0f,		0.0f, 1.0f,
		};

		EEngine::Ref<EEngine::IVertexBuffer> vertexBuffer;
		vertexBuffer = EEngine::RendererAPI::CreateVertexBuffer(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{ EEngine::ShaderData::Float3, "a_Position" },
			{ EEngine::ShaderData::Float4, "a_Color" },
			{ EEngine::ShaderData::Float2, "a_TexCoord" },
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		EEngine::Ref<EEngine::IIndexBuffer> indexBuffer;
		indexBuffer =EEngine::RendererAPI::CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		auto textureShader = EEngine::Renderer::GetShaderLibrary()->Load("assets/shaders/Texture.glsl");
		m_Texture = EEngine::RendererAPI::CreateTexture2D("assets/textures/test.png");

		const auto& openGLShader = std::dynamic_pointer_cast<EEngine::OpenGLShader>(textureShader);
		openGLShader->Bind();
		openGLShader->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(EEngine::Timestep timestep) override {
		EEngine::RendererAPI::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		m_CameraController.OnUpdate(timestep);
		HandleTriMovement(timestep);

		EEngine::Renderer::BeginScene(m_CameraController.GetCamera()); {
			// ER TODO example material usage
			//EEngine::MaterialRef material = new EEngine::Material(m_Shader);
			m_Texture->Bind();
			EEngine::Ref<EEngine::IShader> textureShader;
			if (EEngine::Renderer::GetShaderLibrary()->TryGet("Texture", &textureShader)) {
				EEngine::Renderer::Submit(textureShader, m_VertexArray, glm::translate(glm::mat4(1.0f), m_TriPos));
			}
		} EEngine::Renderer::EndScene();

		// ER TODO usually executed on a separate thread
		//EEngine::Renderer::Flush();
	}

	void OnIMGUIRender() override {

	}

	void OnEvent(EEngine::Event& event) override {
		m_CameraController.OnEvent(event);
	}

	void HandleTriMovement(EEngine::Timestep timestep) {
		float deltaDist = 1.0f * timestep.GetSeconds();

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::I)) {
			m_TriPos.y += deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::J)) {
			m_TriPos.x -= deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::K)) {
			m_TriPos.y -= deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::L)) {
			m_TriPos.x += deltaDist;
		}
	}
private:
	EEngine::Ref<EEngine::IVertexArray> m_VertexArray;
	EEngine::Ref<EEngine::ITexture2D> m_Texture;


	EEngine::CameraController m_CameraController;

	glm::vec3 m_TriPos{};
};

class Runner : public EEngine::Application {
public:
	Runner()
		: Application()
	{
		PushLayer(new ExampleLayer());
	}

	~Runner() override = default;
};

EEngine::Application* EEngine::CreateApplication() {
	return new Runner();
}