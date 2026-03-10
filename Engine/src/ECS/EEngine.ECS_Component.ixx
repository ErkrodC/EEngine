export module EEngine.ECS:Component;
import EEngine.Math;
import EEngine.Standard;

export namespace EEngine {
	struct TagComponent {
		std::string Tag;
	};

	struct TransformComponent {
		Math::Transform Transform;
	};

	struct MeshComponent {
		uint32_t VertexArrayID = 0;
		Math::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct CameraComponent {
		Math::mat4 Projection;
		bool IsPrimary = false;
	};

	struct DirectionalLightComponent {
		Math::vec3 Direction = { -1.0f, -2.0f, -1.0f }; // pointing fwd-down-left
		Math::vec3 Color = { 1.0f, 1.0f, 1.0f };
		float_t ColorIntensity = 1.0f;
		Math::vec3 Ambient = { 1.0f, 1.0f, 1.0f };
		float_t AmbientIntensity = 0.15f;
	};

	struct PointLightComponent {
		Math::vec3 Color = { 1.0f, 1.0f, 1.0f };
		float_t ColorIntensity = 1.0f;
		float_t Radius = 10.0f;
	};

	struct MaterialComponent {
		float_t Metallic = 0.0f; // 0 = dielectric (plastic/wood), 1 = metal
		float_t Roughness = 0.5f; // 0 = mirror-smooth, 1 = completely rough
		Math::vec3 BaseReflectivity = { 0.04f, 0.04f, 0.04f };
	};
}