#include "Renderer.hpp"
#include "RendererAPI.hpp"

namespace EEngine {
	Renderer::API Renderer::s_SelectedAPI = Renderer::API::OpenGL;
	Renderer::SceneData* Renderer::m_SceneData = new SceneData();

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
		const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<IVertexArray>& vertexArray
	) {
		shader->Bind();

		{
			shader->UploadUniformMat4("u_ProjectionView", m_SceneData->ProjectionView);
		}

		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}
} // EEngine