export module EEngine.Rendering:OpenGLContext;
import :IGraphicsContext;

struct GLFWwindow;

export namespace EEngine {
	class OpenGLContext : public IGraphicsContext {
	public:
		explicit OpenGLContext(GLFWwindow* window);

		void Initialize() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;
	};
} // EEngine
