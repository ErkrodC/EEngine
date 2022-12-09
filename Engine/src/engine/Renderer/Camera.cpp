#include "Camera.hpp"

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_float.hpp>
#pragma warning(pop)

namespace EEngine {
	Camera::Camera(const glm::mat4& projection)
		: m_Projection(projection)
		, m_Position(glm::vec3(0.0f, 0.0f, 0.0f))
		, m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {
		RecalculateViewMatrix();
	}

	void Camera::SetProjection(const glm::mat4& projection) {
		m_Projection = projection;
		m_ProjectionView = m_Projection * m_View;
	}

	void Camera::RecalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(m_Rotation);
		m_View = glm::inverse(transform);
		m_ProjectionView = m_Projection * m_View;
	}
} // EEngine