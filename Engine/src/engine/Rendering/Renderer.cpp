module;
#include <memory>
#include <string>
#include "Core/Core.hpp"
#include <glm/ext/matrix_float4x4.hpp>

module EEngine.Rendering:Renderer;
/*import :API;
import :Camera;
import :IShader;
import :IVertexArray;
import :RendererAPI;
import :OpenGLShader;*/

namespace EEngine::Renderer {
	/*Renderer::API Renderer::s_SelectedAPI = Renderer::API::OpenGL;
	Renderer::SceneData* Renderer::s_SceneData = new SceneData();
	Ref<ShaderLibrary> Renderer::s_ShaderLibrary = std::make_shared<ShaderLibrary>();*/

	/*void Initialize() {
		RendererAPI::Initialize();
	}*/

	/*std::string GetRendererAPIString(API api) {
		switch (api) {
			case API::None:
				return "None";
			case API::OpenGL:
				return "OpenGL";
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return "";
	}*/

	/*void BeginScene(const Camera& camera) {
		s_SceneData->ProjectionView = camera.GetProjectionViewMatrix();
	}*/

	/*void EndScene() {

	}*/

	/*void Submit(
		const Ref<IShader>& shader,
		const Ref<IVertexArray>& vertexArray,
		const glm::mat4& transform
	) {
		shader->Bind();

		{
			auto openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
			openGLShader->UploadUniformMat4("u_ProjectionView", s_SceneData->ProjectionView);
			openGLShader->UploadUniformMat4("u_Transform", transform);
		}

		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}*/
} // EEngine