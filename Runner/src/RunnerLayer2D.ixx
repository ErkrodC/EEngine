export module RunnerModule:RunnerLayer2D;
import EEngine;

export class RunnerLayer2D : public EEngine::Layer {
public:
	RunnerLayer2D() : Layer("RunnerLayer2D"), m_CameraController(16.0f / 9.0f) {

	}

	void OnAttach() override {
		m_VertexArray = EEngine::RendererAPI::CreateVertexArray();

		float vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
		};

		EEngine::Ref<EEngine::IVertexBuffer> vertexBuffer;
		vertexBuffer = EEngine::RendererAPI::CreateVertexBuffer(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{ EEngine::ShaderData::Float3, "a_Position" },
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		EEngine::Ref<EEngine::IIndexBuffer> indexBuffer;
		indexBuffer =EEngine::RendererAPI::CreateIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		auto flatColorShader = EEngine::Renderer::GetShaderLibrary()->Load("assets/shaders/FlatColor.glsl");
	}

	void OnDetach() override {

	}

	void OnUpdate(EEngine::Timestep timestep) override {
		m_CameraController.OnUpdate(timestep);

		EEngine::RendererAPI::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		HandleTriMovement(timestep);

		EEngine::Renderer::BeginScene(m_CameraController.GetCamera()); {
			EEngine::Ref<EEngine::IShader> flatColorShader;
			if (EEngine::Renderer::GetShaderLibrary()->TryGet("FlatColor", &flatColorShader)) {
				{ // ER TODO hack while no common api for uploading uniform data
					flatColorShader->Bind();
					auto glFlatColorShader = std::dynamic_pointer_cast<EEngine::OpenGLShader>(flatColorShader);
					glFlatColorShader->UploadUniformFloat4("u_Color", m_SquareColor);
				}

				EEngine::Renderer::Submit(flatColorShader, m_VertexArray, EEngine::Math::translate(EEngine::Math::mat4(1.0f), m_TriPos));
			}
		} EEngine::Renderer::EndScene();

		// ER TODO usually executed on a separate thread
		//EEngine::Renderer::Flush();
	}

	void OnIMGUIRender() override {
		EEngine::Editor::Begin("Settings");
		EEngine::Editor::ColorEdit4("Square Color", EEngine::Math::value_ptr(m_SquareColor));
		EEngine::Editor::End();
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
	EEngine::CameraController m_CameraController;

	// ER TEMP
	EEngine::Ref<EEngine::IVertexArray> m_VertexArray;

	EEngine::Math::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	EEngine::Math::vec3 m_TriPos{};
};