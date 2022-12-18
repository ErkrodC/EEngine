module;
#include "Core/Core.hpp"

export module EEngine.Application:CameraController;
import :Input;
import EEngine.Core;
import EEngine.Event;
import EEngine.Math;
import EEngine.Rendering;

export namespace EEngine {
	class CameraController {
	public:
		explicit CameraController(float aspectRatio)
			: m_AspectRatio(aspectRatio)
			, m_Camera(Math::ortho(
				-m_AspectRatio * m_ZoomLevel,
				m_AspectRatio * m_ZoomLevel,
				-m_ZoomLevel,
				m_ZoomLevel)
		) {

		}

		void OnUpdate(Timestep timestep) {
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

		void OnEvent(Event& event) {
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolled));
			dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
			dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMoved));
		}

		const Camera& GetCamera() {
			return m_Camera;
		}
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		float m_MoveSpeed = 1.0f;
		float m_ZoomSpeed = 0.05f;
		Camera m_Camera;

		bool OnMouseScrolled(MouseScrolledEvent& event) {
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

		bool OnWindowResized(WindowResizeEvent& event) {
			m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
			m_Camera.SetProjection(Math::ortho(
				-m_AspectRatio * m_ZoomLevel,
				m_AspectRatio * m_ZoomLevel,
				-m_ZoomLevel,
				m_ZoomLevel
			));

			return false;
		}

		bool OnMouseMoved(MouseMovedEvent& event) {
			bool isCtrlPressed = EEngine::Input::IsKeyPressed(EEngine::KeyCode::LeftControl)
				|| EEngine::Input::IsKeyPressed(EEngine::KeyCode::RightControl);

			if (isCtrlPressed
				&& EEngine::Input::IsMouseButtonPressed(EEngine::MouseButtonCode::Mouse2)
				) {
				// ER TODO still broken
				/*const auto& cameraRot = m_Camera.GetRotation();
				auto normRot = Math::vec2(event.GetX(), event.GetY());
				normRot = normRot * 0.00001f;
				m_Camera.SetRotation(cameraRot * Math::quat({ *//*normRot.x*//*0.0f, normRot.y, 0.0f }));*/
			}

			return false;
		}
	};
} // EEngine
