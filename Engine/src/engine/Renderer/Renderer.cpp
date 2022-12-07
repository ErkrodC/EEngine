#include <Platform/OpenGL/OpenGLShader.hpp>
#include "Renderer.hpp"
#include "RendererAPI.hpp"

namespace EEngine {
	Renderer::API Renderer::s_SelectedAPI = Renderer::API::OpenGL;
	Renderer::SceneData* Renderer::m_SceneData = new SceneData();

	void Renderer::Initialize() {
		RendererAPI::Initialize();
	}

	std::string Renderer::GetRendererAPIString(Renderer::API api) {
		switch (api) {
			case API::None:
				return "None";
			case API::OpenGL:
				return "OpenGL";
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return "";
	}

	void Renderer::BeginScene(const Camera& camera) {
		m_SceneData->ProjectionView = camera.GetProjectionViewMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(
		const Ref<IShader>& shader,
		const Ref<IVertexArray>& vertexArray,
		const glm::mat4& transform
	) {
		shader->Bind();

		{
			auto openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
			openGLShader->UploadUniformMat4("u_ProjectionView", m_SceneData->ProjectionView);
			openGLShader->UploadUniformMat4("u_Transform", transform);
		}

		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}
} // EEngine