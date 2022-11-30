#pragma once

#include <Renderer/IGraphicsContext.hpp>

struct GLFWwindow;

namespace EEngine {
	class OpenGLContext : public IGraphicsContext {
	public:
		explicit OpenGLContext(GLFWwindow* window);

		void Initialize() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;
	};
} // EEngine
