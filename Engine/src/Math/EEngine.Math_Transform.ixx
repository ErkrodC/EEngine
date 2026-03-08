export module EEngine.Math:Transform;
import :Spatial;

export namespace EEngine::Math {
	struct Transform {
		vec3 Position = { 0.0f, 0.0f, 0.0f };
		quat Rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		vec3 Scale = { 1.0f, 1.0f, 1.0f };

		mat4 GetWorldMatrix() const;
	};
}