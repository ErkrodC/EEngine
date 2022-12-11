module EEngine.Application:CameraController;
import EEngine.Core;
import EEngine.Event;

namespace EEngine {
	CameraController::CameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio)
		, m_Camera(glm::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel)
	) {
		
	}

	void CameraController::OnUpdate(Timestep timestep) {
		{ // Camera movement
			float deltaDist = m_MoveSpeed * timestep.GetSeconds();

			if (Input::IsKeyPressed(EEngine::KeyCode::Up)
				|| Input::IsKeyPressed(EEngine::KeyCode::W)
			) {
				const auto& cameraPos = m_Camera.GetPosition();
				m_Camera.SetPosition({ cameraPos.x, cameraPos.y + deltaDist, cameraPos.z });
			}

			if (Input::IsKeyPressed(EEngine::KeyCode::Left)
				|| Input::IsKeyPressed(EEngine::KeyCode::A)
			) {
				const auto& cameraPos = m_Camera.GetPosition();
				m_Camera.SetPosition({ cameraPos.x - deltaDist, cameraPos.y, cameraPos.z });
			}

			if (Input::IsKeyPressed(EEngine::KeyCode::Down)
				|| Input::IsKeyPressed(EEngine::KeyCode::S)
			) {
				const auto& cameraPos = m_Camera.GetPosition();
				m_Camera.SetPosition({ cameraPos.x, cameraPos.y - deltaDist, cameraPos.z });
			}

			if (Input::IsKeyPressed(EEngine::KeyCode::Right)
				|| Input::IsKeyPressed(EEngine::KeyCode::D)
			) {
				const auto& cameraPos = m_Camera.GetPosition();
				m_Camera.SetPosition({ cameraPos.x + deltaDist, cameraPos.y, cameraPos.z });
			}
		}
	}

	void CameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMoved));
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& event) {
		m_ZoomLevel -= event.GetYOffset() * m_ZoomSpeed;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.01f);

		m_Camera.SetProjection(glm::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel
		));

		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& event) {
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(glm::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel
		));

		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& event) {
		bool isCtrlPressed = EEngine::Input::IsKeyPressed(EEngine::KeyCode::LeftControl)
			|| EEngine::Input::IsKeyPressed(EEngine::KeyCode::RightControl);

		if (isCtrlPressed
			&& EEngine::Input::IsMouseButtonPressed(EEngine::MouseButtonCode::Mouse2)
		) {
			// ER TODO still broken
			/*const auto& cameraRot = m_Camera.GetRotation();
			auto normRot = glm::vec2(event.GetX(), event.GetY());
			normRot = normRot * 0.00001f;
			m_Camera.SetRotation(cameraRot * glm::quat({ *//*normRot.x*//*0.0f, normRot.y, 0.0f }));*/
		}

		return false;
	}

	const Camera& CameraController::GetCamera() {
		return m_Camera;
	}
} // EEngine