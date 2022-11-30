#include <Renderer/Renderer.hpp>
#include "EEngine.hpp"
#include "KeyCode.hpp"
#include "imgui.h"

class ExampleLayer : public EEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override {
		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::Tab)) {
			EE_TRACE("TAb key is pressed (poll)!");
		}
	}

	void OnIMGUIRender() override {
		/*ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();*/
	}

	void OnEvent(EEngine::Event& event) override {
		//EE_TRACE(event);
	}
};

class Runner : public EEngine::Application {
public:
	Runner() {
		m_VertexArray.reset(EEngine::IVertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<EEngine::IVertexBuffer> vertexBuffer;
		vertexBuffer.reset(EEngine::IVertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{ EEngine::ShaderData::Float3, "a_Position" },
			{ EEngine::ShaderData::Float4, "a_Color" },
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<EEngine::IIndexBuffer> indexBuffer;
		indexBuffer.reset(EEngine::IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = v_Color;
			}
		)";

		m_Shader = std::make_unique<EEngine::Shader>(vertexSource, fragmentSource);

		PushLayer(new ExampleLayer());
	}

	~Runner() override {}
protected:
	void OnSceneUpdate() override {
		m_Shader->Bind();
		EEngine::Renderer::Submit(m_VertexArray);
	}
private:
	std::shared_ptr<EEngine::Shader> m_Shader;
	std::shared_ptr<EEngine::IVertexArray> m_VertexArray;
};

EEngine::Application* EEngine::CreateApplication() {
	return new Runner();
}