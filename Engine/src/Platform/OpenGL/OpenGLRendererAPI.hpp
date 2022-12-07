#pragma once

#include <Renderer/IRendererAPI.hpp>

namespace EEngine {
	class OpenGLRendererAPI : public IRendererAPI {
	public:
	private:
		void InitializeImpl() override;
	public:
		void ClearImpl(const glm::vec4& color) override;
		void DrawIndexedImpl(const Ref<IVertexArray>& vertexArray) override;
	};
} // EEngine
