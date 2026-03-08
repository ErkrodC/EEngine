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
		explicit CameraController(float_t aspectRatio, Input& input);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		const Camera& GetCamera() { return m_Camera; }
	private:
		float_t m_AspectRatio;
		float_t m_ZoomLevel = 1.0f;
		float_t m_MoveSpeed = 1.0f;
		float_t m_ZoomSpeed = 0.05f;
		Camera m_Camera;
		Input& m_Input;

		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event) const;
	};
}

// ============================================================================
// CameraController Implementation
// ============================================================================
namespace EEngine {
	CameraController::CameraController(float_t aspectRatio, Input& input)
		: m_AspectRatio(aspectRatio)
		, m_Camera(Math::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel))
		, m_Input(input)
	{}

	void CameraController::OnUpdate(Timestep timestep) {
		double_t deltaDist = m_MoveSpeed * timestep.GetSeconds();

		/*if (m_Input.IsKeyPressed(KeyCode::Up) || m_Input.IsKeyPressed(KeyCode::W)) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x, cameraPos.y + deltaDist, cameraPos.z });
		}

		if (m_Input.IsKeyPressed(KeyCode::Left) || m_Input.IsKeyPressed(KeyCode::A)) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x - deltaDist, cameraPos.y, cameraPos.z });
		}

		if (m_Input.IsKeyPressed(KeyCode::Down) || m_Input.IsKeyPressed(KeyCode::S)) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x, cameraPos.y - deltaDist, cameraPos.z });
		}

		if (m_Input.IsKeyPressed(KeyCode::Right) || m_Input.IsKeyPressed(KeyCode::D)) {
			const auto& cameraPos = m_Camera.GetPosition();
			m_Camera.SetPosition({ cameraPos.x + deltaDist, cameraPos.y, cameraPos.z });
		}*/
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
		m_AspectRatio = static_cast<float_t>(event.GetWidth()) / static_cast<float_t>(event.GetHeight());
		m_Camera.SetProjection(Math::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel
		));

		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& event) const {
		bool isCtrlPressed = m_Input.IsKeyPressed(KeyCode::LeftControl)
			|| m_Input.IsKeyPressed(KeyCode::RightControl);

		if (isCtrlPressed && m_Input.IsMouseButtonPressed(MouseButtonCode::Mouse2)) {
			// ER TODO camera rotation still broken
		}

		return false;
	}
}
