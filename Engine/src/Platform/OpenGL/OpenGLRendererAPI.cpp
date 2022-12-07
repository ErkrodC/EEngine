#include <glad/glad.h>
#include "OpenGLRendererAPI.hpp"

namespace EEngine {
	void OpenGLRendererAPI::InitializeImpl() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::ClearImpl(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexedImpl(const Ref<IVertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES,
			(GLsizei)vertexArray->GetIndexBuffer()->GetCount(),
			GL_UNSIGNED_INT,
			nullptr
		);
	}
} // EEngine