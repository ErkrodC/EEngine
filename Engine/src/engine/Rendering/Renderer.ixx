module;
#include "Core/Core.hpp"

export module EEngine.Rendering:Renderer;
import :API;
import :Buffer;
import :Camera;
import :IShader;
import :IVertexArray;
import :RendererAPI;
import :ShaderLibrary;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;

namespace EEngine::Renderer {
	struct SceneData {
		Math::mat4 ProjectionView;
	};

	inline SceneData& GetSceneData() {
		static SceneData instance;
		return instance;
	}

	inline Ref<ShaderLibrary>& GetShaderLibraryInstance() {
		static Ref<ShaderLibrary> instance = CreateRef<ShaderLibrary>();
		return instance;
	}

	static API s_SelectedAPI = API::OpenGL;

	export void Initialize() {
		RendererAPI::Initialize();
	}

	export void OnWindowResized(uint32_t width, uint32_t height) {
		RendererAPI::SetViewport(0, 0, width, height);
	}

	export void BeginScene(const Camera& camera) {
		GetSceneData().ProjectionView = camera.GetProjectionViewMatrix();
	}

	export void EndScene() {

	}

	export void Submit(
		const Ref<IShader>& shader,
		const Ref<IVertexArray>& vertexArray,
		const Math::mat4& transform = Math::mat4(1.0f)
	) {
		shader->Bind();
		shader->SetMat4("u_ProjectionView", GetSceneData().ProjectionView);
		shader->SetMat4("u_Transform", transform);
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

		Log::CoreError("Unknown rendering API.");
		return "";
	}

	export inline Ref<ShaderLibrary> GetShaderLibrary() { return GetShaderLibraryInstance(); }
}; // EEngine
