module;
#include "Core/Core.hpp"
DISABLE_WARNING_PUSH
DISABLE_WARNING_NAMELESS_STRUCT_UNION
#include <glm/mat4x4.hpp>
DISABLE_WARNING_POP

export module EEngine.Rendering:Renderer;
import :API;
import :Buffer;
import :Camera;
import :IShader;
import :IVertexArray;
import :OpenGLShader;
import :RendererAPI;
import :ShaderLibrary;
import EEngine.Core;
import EEngine.std.core;

namespace EEngine::Renderer {
	struct SceneData {
		glm::mat4 ProjectionView;
	};

	/*static API s_SelectedAPI;
	static SceneData* s_SceneData;
	static Ref<ShaderLibrary> s_ShaderLibrary;*/

	static API s_SelectedAPI = API::OpenGL;
	static SceneData* s_SceneData = new SceneData();
	static Ref<ShaderLibrary> s_ShaderLibrary = std::make_shared<ShaderLibrary>();

	export void Initialize() {
		RendererAPI::Initialize();
	}

	export void OnWindowResized(uint32_t width, uint32_t height) {
		RendererAPI::SetViewport(0, 0, width, height);
	}

	export void BeginScene(const Camera& camera) {
		s_SceneData->ProjectionView = camera.GetProjectionViewMatrix();
	}

	export void EndScene() {

	}

	export void Submit(
		const Ref<IShader>& shader,
		const Ref<IVertexArray>& vertexArray,
		const glm::mat4& transform = glm::mat4(1.0f)
	) {
		shader->Bind();

		{
			auto openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
			openGLShader->UploadUniformMat4("u_ProjectionView", s_SceneData->ProjectionView);
			openGLShader->UploadUniformMat4("u_Transform", transform);
		}

		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}

	export inline API GetSelectedAPI() { return s_SelectedAPI; }
	export std::string GetRendererAPIString(API api) {
		switch (api) {
			case API::None:
				return "None";
			case API::OpenGL:
				return "OpenGL";
		}

		EE_CORE_ERROR("Unknown rendering API.");
		return "";
	}

	export inline Ref<ShaderLibrary> GetShaderLibrary() { return s_ShaderLibrary; }
}; // EEngine
