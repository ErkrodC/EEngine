module;
#include "Core/Core.hpp"
DISABLE_WARNING_PUSH
DISABLE_WARNING_NAMELESS_STRUCT_UNION
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_float.hpp>
DISABLE_WARNING_POP

export module EEngine.Rendering:Camera;

export namespace EEngine {
	class Camera {
	public:
		explicit Camera(const glm::mat4& projection)
			: m_Projection(projection)
			, m_Position(glm::vec3(0.0f, 0.0f, 0.0f))
			, m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
		) {
			RecalculateViewMatrix();
		}

		void SetProjection(const glm::mat4& projection) {
			m_Projection = projection;
			m_ProjectionView = m_Projection * m_View;
		}

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::quat& GetRotation() const { return m_Rotation; }
		void SetRotation(const glm::quat& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }


		const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
		const glm::mat4& GetViewMatrix() const { return m_View; }
		const glm::mat4& GetProjectionViewMatrix() const { return m_ProjectionView; }
	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_ProjectionView;

		glm::vec3 m_Position;
		glm::quat m_Rotation;

		void RecalculateViewMatrix() {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(m_Rotation);
			m_View = glm::inverse(transform);
			m_ProjectionView = m_Projection * m_View;
		}
	};
} // EEngine
