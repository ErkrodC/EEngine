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

		using ViewReturn = Registry::ViewReturn;
		// 2. Apply the first directional light found
		m_Registry.View<DirectionalLightComponent>([&renderer](uint32_t, DirectionalLightComponent& directionalLight) {
			renderer.SetDirectionalLight(
				directionalLight.Direction,
				directionalLight.Color,
				directionalLight.ColorIntensity,
				directionalLight.Ambient,
				directionalLight.AmbientIntensity
			);
			return ViewReturn::Break;
		});

		// 3. Collect point lights
		uint32_t pointLightIndex = 0;
		m_Registry.View<TransformComponent, PointLightComponent>([&renderer, &pointLightIndex](uint32_t, TransformComponent& transform, PointLightComponent& pointLight) {
			if (pointLightIndex >= MAX_POINT_LIGHTS) { return ViewReturn::Continue; }

			renderer.SetPointLight(
				pointLightIndex++,
				transform.Transform.Position,
				pointLight.Radius,
				pointLight.Color,
				pointLight.ColorIntensity
			);

			return ViewReturn::Continue;
		});
		renderer.SetPointLightCount(pointLightIndex);

		// 4. Submit all mesh entities
		m_Registry.View<TransformComponent, MeshComponent>([&](uint32_t entity, const TransformComponent& transform, const MeshComponent& mesh) {
			Shared<VertexArray> vertexArray = GetMesh(mesh.VertexArrayID);
			if (!vertexArray) { return ViewReturn::Continue; }

			float_t metallic = 0.0f;
			float_t roughness = 0.5f;
			if (m_Registry.Has<MaterialComponent>(entity)) {
				const auto& material = m_Registry.Get<MaterialComponent>(entity);
				metallic = material.Metallic;
				roughness = material.Roughness;
			}

			renderer.SubmitMesh(
				vertexArray,
				transform.Transform.GetWorldMatrix(),
				mesh.Color,
				metallic,
				roughness
			);

			return ViewReturn::Continue;
		});

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
		m_MeshByID[id] = vertexArray;
		return id;
	}

	Shared<VertexArray> Scene::GetMesh(uint32_t id) const {
		auto it = m_MeshByID.find(id);
		if (it != m_MeshByID.end()) { return it->second; }
		return nullptr;
	}
}