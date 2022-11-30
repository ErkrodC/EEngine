#include "Application.hpp"
#include <glad/glad.h>
#include <Renderer/Shader.hpp>
#include <memory>

#include "Input.hpp"

namespace EEngine {
	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataToOpenGLBaseType(ShaderData type) {
		switch (type) {
			case ShaderData::None:		break;

			case ShaderData::Float:
			case ShaderData::Float2:
			case ShaderData::Float3:
			case ShaderData::Float4:
			case ShaderData::Mat3:
			case ShaderData::Mat4:		return GL_FLOAT;

			case ShaderData::Int:
			case ShaderData::Int2:
			case ShaderData::Int3:
			case ShaderData::Int4:			return GL_INT;

			case ShaderData::Bool:			return GL_BOOL;
		}

		EE_CORE_ERROR("Unknown shader data type.");
		return GL_NONE;
	}

	Application::Application() {
		EE_CORE_ASSERT(!s_Instance, "Multiple applications created.");

		Application::s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_IMGUILayer = new IMGUILayer();
		PushOverlay(m_IMGUILayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderData::Float3, "a_Position" },
			{ ShaderData::Float4, "a_Color" },
		};

		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index++,
				(GLint)element.ComponentCount,
				ShaderDataToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				(GLsizei)layout.GetStride(),
				(const void *)(intptr_t)element.Offset
			);
		}

		m_VertexBuffer->SetLayout(layout);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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