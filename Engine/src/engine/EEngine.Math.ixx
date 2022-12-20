// ER NOTE this header projection header unit is a temporary while header units
//  (esp std lib ones) are being worked on in msvc + cmake: https://gitlab.kitware.com/cmake/cmake/-/issues/18355
module;
#include "Core/Core.hpp"
DISABLE_WARNING_PUSH
DISABLE_WARNING_NAMELESS_STRUCT_UNION
#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_float4x4_precision.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_mat4x4.hpp>
DISABLE_WARNING_POP

export module EEngine.Math;

export namespace EEngine::Math {
	using glm::inverse;
	using glm::mat;
	using glm::mat3;
	using glm::mat4;
	using glm::ortho;
	using glm::quat;
	using glm::operator*;
	using glm::qualifier;
	using glm::scale;
	using glm::toMat4;
	using glm::translate;
	using glm::value_ptr;
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;

	namespace Identity {
		const ::EEngine::Math::mat4 mat4 = Math::mat4(1.0f) * Math::mat4(1.0f);
	}

	constexpr uint32_t Bit(uint32_t x) {
		return 1 << x;
	}
}