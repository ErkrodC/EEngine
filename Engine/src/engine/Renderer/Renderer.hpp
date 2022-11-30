#pragma once

#include <glm/vec4.hpp>
#include "Buffer.hpp"
#include "IRendererAPI.hpp"

namespace EEngine {
	class Renderer {
	public:
		enum class API {
			None = 0, OpenGL = 1,
		};

		static std::string GetRendererAPIString(API api);

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static API GetSelectedAPI() { return s_SelectedAPI; }

	private:
		static API s_SelectedAPI;
	};
} // EEngine
