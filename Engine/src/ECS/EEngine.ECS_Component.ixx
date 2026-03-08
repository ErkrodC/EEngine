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
}