module;
#include <glm/detail/type_mat4x4.hpp>

module EEngine.Math;
import :Transform;

namespace EEngine::Math {
	mat4 Transform::GetWorldMatrix() const {
		return translate(mat4(1.0f), Position)
			* toMat4(quat(Rotation))
			* scale(mat4(1.0f), Scale);
	}
}