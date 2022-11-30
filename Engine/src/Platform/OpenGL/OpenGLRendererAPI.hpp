#pragma once

#include <Renderer/IRendererAPI.hpp>

namespace EEngine {
	class OpenGLRendererAPI : public IRendererAPI {
	public:
		void ClearImpl(const glm::vec4& color) override;
		void DrawIndexedImpl(const std::shared_ptr<IVertexArray>& vertexArray) override;
	};
} // EEngine
