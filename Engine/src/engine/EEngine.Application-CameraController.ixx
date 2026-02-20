export module EEngine.Application:CameraController;
import EEngine.Core;
import EEngine.Event;
import EEngine.Math;
import EEngine.Rendering;
import EEngine.Standard;
import :Input;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Camera Controller
	// ============================================================================

	class CameraController {
	public:
		explicit CameraController(float aspectRatio);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		const Camera& GetCamera() { return m_Camera; }
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
}

// ============================================================================
// CameraController Implementation
// ============================================================================

namespace EEngine {
	CameraController::CameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio)
		, m_Camera(Math::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel)
	) {}

	void CameraController::OnUpdate(Timestep timestep) {
		float deltaDist = m_MoveSpeed * timestep.GetSeconds();

		if (Input::IsKeyPressed(KeyCode::Up) || Input::IsKeyPressed(KeyCode::W)) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x, cameraPos.y + deltaDist, cameraPos.z });
		}

		if (Input::IsKeyPressed(KeyCode::Left) || Input::IsKeyPressed(KeyCode::A)) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x - deltaDist, cameraPos.y, cameraPos.z });
		}

		if (Input::IsKeyPressed(KeyCode::Down) || Input::IsKeyPressed(KeyCode::S)) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x, cameraPos.y - deltaDist, cameraPos.z });
		}

		if (Input::IsKeyPressed(KeyCode::Right) || Input::IsKeyPressed(KeyCode::D)) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x + deltaDist, cameraPos.y, cameraPos.z });
		}
	}

	void CameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>([this](auto& event) -> bool { return OnMouseScrolled(event); });
		dispatcher.Dispatch<WindowResizeEvent>([this](auto& event) -> bool { return OnWindowResized(event); });
		dispatcher.Dispatch<MouseMovedEvent>([this](auto& event) -> bool { return OnMouseMoved(event); });
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& event) {
		m_ZoomLevel -= event.GetYOffset() * m_ZoomSpeed;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.01f);

		m_Camera.SetProjection(Math::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel
		));

		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& event) {
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(Math::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel
		));

		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& event) {
		bool isCtrlPressed = Input::IsKeyPressed(KeyCode::LeftControl)
			|| Input::IsKeyPressed(KeyCode::RightControl);

		if (isCtrlPressed && Input::IsMouseButtonPressed(MouseButtonCode::Mouse2)) {
			// ER TODO camera rotation still broken
		}

		return false;
	}
}
