#include "Renderer.hpp"
#include "RendererAPI.hpp"

namespace EEngine {
	Renderer::API Renderer::s_SelectedAPI = Renderer::API::OpenGL;

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

	void Renderer::BeginScene() {

	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const std::shared_ptr<IVertexArray>& vertexArray) {
		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}
} // EEngine