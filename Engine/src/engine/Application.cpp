#include "Application.hpp"
#include <Renderer/Shader.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/RendererAPI.hpp>
#include <memory>

#include "Input.hpp"

namespace EEngine {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		EE_CORE_INFO("Selected Renderer API: {0}", Renderer::GetRendererAPIString(Renderer::GetSelectedAPI()));
		EE_CORE_ASSERT(!s_Instance, "Multiple applications created.");

		Application::s_Instance = this;

		m_Window = std::unique_ptr<IWindow>(IWindow::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_IMGUILayer = new IMGUILayer();
		PushOverlay(m_IMGUILayer);

		m_VertexArray.reset(IVertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<IVertexBuffer> vertexBuffer;
		vertexBuffer.reset(IVertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{ ShaderData::Float3, "a_Position" },
			{ ShaderData::Float4, "a_Color" },
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IIndexBuffer> indexBuffer;
		indexBuffer.reset(IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader = std::make_unique<Shader>(vertexSource, fragmentSource);
	}

	Application::~Application() = default;

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(event);
			if (event.Handled) { break; }
		}
	}

	void Application::Run() {
		while (m_Running) {
			RendererAPI::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

			Renderer::BeginScene(); {
				m_Shader->Bind();
				Renderer::Submit(m_VertexArray);
			} Renderer::EndScene();

			// ER TODO usually executed on a separate thread
			//Renderer::Flush();

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			// ER TODO will eventually be on a "render" thread, isolated from update/application thread
			m_IMGUILayer->Begin();
			for (Layer* layer: m_LayerStack) {
				layer->OnIMGUIRender();
			}
			m_IMGUILayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(EEngine::WindowCloseEvent&) {
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(EEngine::Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
} // Engine