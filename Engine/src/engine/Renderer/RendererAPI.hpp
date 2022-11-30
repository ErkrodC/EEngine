#pragma once

#include "IRendererAPI.hpp"

namespace EEngine {
	class RendererAPI {
	public:
		static inline void Clear(const glm::vec4& color) {
			s_RendererAPI->ClearImpl(color);
		}

		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			s_RendererAPI->DrawIndexedImpl(vertexArray);
		}
	private:
		static IRendererAPI* s_RendererAPI;
	};
} // EEngine
