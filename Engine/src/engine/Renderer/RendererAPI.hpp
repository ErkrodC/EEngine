#pragma once

#include "IRendererAPI.hpp"

namespace EEngine {
	class RendererAPI {
	public:
		static inline void Initialize() {
			s_RendererAPI->InitializeImpl();
		}

		static inline void Clear(const glm::vec4& color) {
			s_RendererAPI->ClearImpl(color);
		}

		static inline void DrawIndexed(const Ref<IVertexArray>& vertexArray) {
			s_RendererAPI->DrawIndexedImpl(vertexArray);
		}
	private:
		static IRendererAPI* s_RendererAPI;
	};
} // EEngine
