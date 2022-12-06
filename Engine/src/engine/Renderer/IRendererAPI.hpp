#pragma once

#include <glm/vec4.hpp>
#include "IVertexArray.hpp"

namespace EEngine {
	class IRendererAPI {
		friend class RendererAPI;
	private:
		virtual void ClearImpl(const glm::vec4& color) = 0;
		virtual void DrawIndexedImpl(const Ref<IVertexArray>& vertexArray) = 0;
	};
} // EEngine
