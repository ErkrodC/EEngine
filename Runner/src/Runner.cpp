#include <glm/ext/matrix_clip_space.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Camera.hpp>
#include <Events/Event.hpp>
#include <Platform/OpenGL/OpenGLShader.hpp>
#include "EEngine.hpp"

class ExampleLayer : public EEngine::Layer {
public:
	ExampleLayer()
			: Layer("Example")
			, m_Camera(glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f)) {
			//, m_Camera(glm::perspective(glm::radians(120.0f), 9.0f/16.0f/*/9.0f*/, 0.01f, 100.0f)) {
		m_VertexArray = EEngine::IVertexArray::Create();

		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,		0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.8f, 1.0f,		1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.8f, 0.8f, 0.8f, 1.0f,		0.0f, 1.0f,
		};

		EEngine::Ref<EEngine::IVertexBuffer> vertexBuffer;
		vertexBuffer = EEngine::IVertexBuffer::Create(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{ EEngine::ShaderData::Float3, "a_Position" },
			{ EEngine::ShaderData::Float4, "a_Color" },
			{ EEngine::ShaderData::Float2, "a_TexCoord" },
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		EEngine::Ref<EEngine::IIndexBuffer> indexBuffer;
		indexBuffer =EEngine::IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;

			uniform mat4 u_ProjectionView;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			out vec2 v_TexCoord;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				v_TexCoord = a_TexCoord;
				gl_Position = u_ProjectionView * u_Transform *  vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main() {
				//color = v_Color;
				//color = vec4(v_TexCoord, 0.0, 1.0);
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_Shader = EEngine::IShader::Create(vertexSource, fragmentSource);
		m_Texture = EEngine::ITexture2D::Create("assets/textures/test.png");

		const auto& openGLShader = std::dynamic_pointer_cast<EEngine::OpenGLShader>(m_Shader);
		openGLShader->Bind();
		openGLShader->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(EEngine::Timestep timestep) override {
		EEngine::RendererAPI::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		HandleCameraMovement(timestep);
		HandleTriMovement(timestep);

		EEngine::Renderer::BeginScene(m_Camera); {
			// ER TODO example material usage
			//EEngine::MaterialRef material = new EEngine::Material(m_Shader);
			m_Texture->Bind();
			EEngine::Renderer::Submit(m_Shader, m_VertexArray, glm::translate(glm::mat4(1.0f), m_TriPos));
		} EEngine::Renderer::EndScene();

		// ER TODO usually executed on a separate thread
		//EEngine::Renderer::Flush();
	}

	void OnIMGUIRender() override {

	}

	void OnEvent(EEngine::Event& event) override {
		EEngine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<EEngine::MouseMovedEvent>(BIND_EVENT_FN(OnMouseMoved));
	}

	bool OnMouseMoved(EEngine::MouseMovedEvent& event) {
		bool isCtrlPressed = EEngine::Input::IsKeyPressed(EEngine::KeyCode::LeftControl)
			|| EEngine::Input::IsKeyPressed(EEngine::KeyCode::RightControl);

		if (isCtrlPressed
			&& EEngine::Input::IsMouseButtonPressed(EEngine::MouseButtonCode::Mouse2)
		) {
			// ER TODO still broken
			const auto& cameraRot = m_Camera.GetRotation();
			auto normRot = glm::vec2(event.GetX(), event.GetY());
			normRot = normRot * 0.00001f;
			m_Camera.SetRotation(cameraRot * glm::quat({ /*normRot.x*/0.0f, normRot.y, 0.0f }));
		}

		return false;
	}

	void HandleCameraMovement(EEngine::Timestep timestep) {
		float deltaDist = 1.0f * timestep.GetSeconds();

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::Up)
			|| EEngine::Input::IsKeyPressed(EEngine::KeyCode::W)
		) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x, cameraPos.y + deltaDist, cameraPos.z });
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::Left)
			|| EEngine::Input::IsKeyPressed(EEngine::KeyCode::A)
		) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x - deltaDist, cameraPos.y, cameraPos.z });
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::Down)
			|| EEngine::Input::IsKeyPressed(EEngine::KeyCode::S)
		) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x, cameraPos.y - deltaDist, cameraPos.z });
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::Right)
			|| EEngine::Input::IsKeyPressed(EEngine::KeyCode::D)
		) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x + deltaDist, cameraPos.y, cameraPos.z });
		}
	}

	void HandleTriMovement(EEngine::Timestep timestep) {
		float deltaDist = 1.0f * timestep.GetSeconds();

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::I)
			) {
			m_TriPos.y += deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::J)
			) {
			m_TriPos.x -= deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::K)
			) {
			m_TriPos.y -= deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::L)
			) {
			m_TriPos.x += deltaDist;
		}
	}
private:
	EEngine::Ref<EEngine::IShader> m_Shader;
	EEngine::Ref<EEngine::IVertexArray> m_VertexArray;
	EEngine::Ref<EEngine::ITexture2D> m_Texture;


	EEngine::Camera m_Camera;

	glm::vec3 m_TriPos{};
};

class Runner : public EEngine::Application {
public:
	Runner()
		: Application()
	{
		PushLayer(new ExampleLayer());
	}

	~Runner() override {}
};

EEngine::Application* EEngine::CreateApplication() {
	return new Runner();
}