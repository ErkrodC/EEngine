#pragma once

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#pragma warning(pop)

#undef near
#undef far

namespace EEngine {
	class Camera {
	public:
		explicit Camera(
			const glm::mat4& projection,
			const glm::vec3 position = glm::vec3(0.0f)
		);

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

		void RecalculateViewMatrix();
	};
} // EEngine
