#pragma once

#include <Renderer/GraphicsContext.hpp>

struct GLFWwindow;

namespace EEngine {
	class OpenGLContext : public GraphicsContext {
	public:
		explicit OpenGLContext(GLFWwindow* window);

		void Initialize() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;
	};
} // EEngine
