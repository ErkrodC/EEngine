export module EEngine.Application:IMGUILayer;
import :IWindow;
import :Layer;

export namespace EEngine {
	class IMGUILayer : public Layer {
	public:
		IMGUILayer(IWindow*);
		~IMGUILayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnIMGUIRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
		IWindow* m_Window;
	};
} // EEngine
