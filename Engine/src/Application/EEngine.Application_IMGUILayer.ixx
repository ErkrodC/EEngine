export module EEngine.Application:IMGUILayer;
import EEngine.Core;
import EEngine.Standard;
import :Layer;
import :Window;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// ImGui Layer
	// ============================================================================
	class IMGUILayer : public Layer {
	public:
		IMGUILayer(Shared<Window> window) : Layer("IMGUILayer"), m_Window(window) {}
		~IMGUILayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnIMGUIRender() override;

		void Begin();
		void End();
	private:
		float_t m_Time = 0.0f;
		Shared<Window> m_Window;
	};
}
