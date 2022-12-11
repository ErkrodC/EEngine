export module EEngine.Application:CameraController;
import EEngine.Core;
import EEngine.Event;
import EEngine.Rendering;

export namespace EEngine {
	class CameraController {
	public:
		explicit CameraController(float aspectRatio);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);
		const Camera& GetCamera();
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		float m_MoveSpeed = 1.0f;
		float m_ZoomSpeed = 0.05f;
		Camera m_Camera;

		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
	};
} // EEngine
