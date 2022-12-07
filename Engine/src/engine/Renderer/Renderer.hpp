#pragma once

#include <glm/vec4.hpp>
#include "Buffer.hpp"
#include "IRendererAPI.hpp"
#include "Camera.hpp"
#include "IShader.hpp"

namespace EEngine {
	class Renderer {
	public:
		enum class API {
			None = 0, OpenGL = 1,
		};

		static void Initialize();

		static void BeginScene(const EEngine::Camera& camera);
		static void EndScene();

		static void Submit(
			const Ref<IShader>& shader,
			const Ref<IVertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f)
		);

		inline static API GetSelectedAPI() { return s_SelectedAPI; }
		static std::string GetRendererAPIString(API api);

		static Ref<ShaderLibrary> GetShaderLibrary() { return s_ShaderLibrary; }
	private:
		struct SceneData {
			glm::mat4 ProjectionView;
		};

		static SceneData* s_SceneData;
		static API s_SelectedAPI;
		static Ref<ShaderLibrary> s_ShaderLibrary;
	};
} // EEngine
