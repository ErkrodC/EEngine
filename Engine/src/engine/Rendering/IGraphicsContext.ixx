export module EEngine.Rendering:IGraphicsContext;

export namespace EEngine {
	class IGraphicsContext {
	public:
		virtual void Initialize() = 0;
		virtual void SwapBuffers() = 0;
	};
} // EEngine
