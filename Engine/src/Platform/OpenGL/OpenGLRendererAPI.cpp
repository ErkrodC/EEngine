#include <glad/glad.h>
#include "OpenGLRendererAPI.hpp"

namespace EEngine {
	void OpenGLRendererAPI::ClearImpl(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexedImpl(const std::shared_ptr<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES,
			(GLsizei)vertexArray->GetIndexBuffer()->GetCount(),
			GL_UNSIGNED_INT,
			nullptr
		);
	}
} // EEngine