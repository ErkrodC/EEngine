module;
#include <glm/detail/type_mat4x4.hpp>

module EEngine.ECS;
import :Scene;

using namespace EEngine::Rendering;

namespace EEngine {
	SceneEntity Scene::CreateEntity(const std::string& name) {
		SceneEntity entity = { ++m_EntityCount, this };
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(SceneEntity entity) {
		m_Registry.Remove(entity);
	}

	void Scene::OnUpdate(Timestep ts) {
		// Systems run here (physics, AI, animation, etc.)
		// Currently no systems - placeholder for future work.
	}

	void Scene::OnRender(Renderer& renderer) {
		// 1. Find the primary camera and derive the view-projection
		SceneEntity cameraEntity = GetPrimaryCameraEntity();
		if (!cameraEntity) { return; }

		auto& cam = m_Registry.Get<CameraComponent>(cameraEntity);
		auto& camTransform = m_Registry.Get<TransformComponent>(cameraEntity);

		Math::mat4 view = Math::inverse(camTransform.Transform.GetWorldMatrix());
		Math::mat4 viewProjection = cam.Projection * view;

		renderer.BeginScene(viewProjection);

		// 2. Submit all mesh entities
		auto entities = m_Registry.View<TransformComponent, MeshComponent>();
		for (auto entityID : entities) {
			auto& transform = m_Registry.Get<TransformComponent>(entityID);
			auto& mesh = m_Registry.Get<MeshComponent>(entityID);

			Shared<VertexArray> vertexArray = GetMesh(mesh.VertexArrayID);
			if (!vertexArray) { continue; }

			renderer.SubmitMesh(vertexArray, transform.Transform.GetWorldMatrix());
		}

		renderer.EndScene();
	}

	SceneEntity Scene::GetPrimaryCameraEntity() {
		auto entities = m_Registry.View<CameraComponent>();
		for (auto entityID : entities) {
			auto& camera = m_Registry.Get<CameraComponent>(entityID);
			if (camera.IsPrimary) {
				return SceneEntity{ entityID, this };
			}
		}

		return {};
	}

	uint32_t Scene::RegisterMesh(const Shared<VertexArray>& vertexArray) {
		uint32_t id = ++m_NextMeshID;
		m_MeshLookup[id] = vertexArray;
		return id;
	}

	Shared<VertexArray> Scene::GetMesh(uint32_t id) const {
		auto it = m_MeshLookup.find(id);
		if (it != m_MeshLookup.end()) { return it->second; }
		return nullptr;
	}
}