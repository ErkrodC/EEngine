module;
// Workaround: GLM operators don't get found via ADL when re-exported with 'using' declarations.
// Need direct include in global module fragment for operator* to work on glm::mat4.
#include <glm/detail/type_mat4x4.hpp>

export module EEngine.Rendering:Camera;
import EEngine.Math;

export namespace EEngine {
	class Camera {
	public:
		explicit Camera(const Math::mat4& projection)
			: m_Projection(projection)
			, m_Position(Math::vec3(0.0f, 0.0f, 0.0f))
			, m_Rotation(Math::quat(1.0f, 0.0f, 0.0f, 0.0f)
		) {
			RecalculateViewMatrix();
		}

		void SetProjection(const Math::mat4& projection) {
			m_Projection = projection;
			m_ProjectionView = m_Projection * m_View;
		}

		const Math::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const Math::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const Math::quat& GetRotation() const { return m_Rotation; }
		void SetRotation(const Math::quat& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }


		const Math::mat4& GetProjectionMatrix() const { return m_Projection; }
		const Math::mat4& GetViewMatrix() const { return m_View; }
		const Math::mat4& GetProjectionViewMatrix() const { return m_ProjectionView; }
	private:
		Math::mat4 m_Projection;
		Math::mat4 m_View;
		Math::mat4 m_ProjectionView;

		Math::vec3 m_Position;
		Math::quat m_Rotation;

		void RecalculateViewMatrix() {
			Math::mat4 transform = Math::translate(Math::mat4(1.0f), m_Position) * Math::toMat4(m_Rotation);
			m_View = Math::inverse(transform);
			m_ProjectionView = m_Projection * m_View;
		}
	};
} // EEngine
