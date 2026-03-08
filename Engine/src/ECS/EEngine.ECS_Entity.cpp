module EEngine.ECS;
import :Entity;

namespace EEngine {
	template<typename T, typename... Args>
	T& SceneEntity::AddComponent(Args&&... args) {
		return m_Scene->m_Registry.Add<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& SceneEntity::GetComponent() const {
		return m_Scene->m_Registry.Get<T>(m_EntityHandle);
	}

	template<typename T>
	bool SceneEntity::HasComponent() const {
		return m_Scene->m_Registry.Has<T>(m_EntityHandle);
	}

	// Explicit instantiations for the component types we use
	template TagComponent& SceneEntity::AddComponent<TagComponent, std::string>(std::string&&);
	template TagComponent& SceneEntity::AddComponent<TagComponent, const std::string&>(const std::string&);
	template TransformComponent& SceneEntity::AddComponent<TransformComponent>();
	template CameraComponent& SceneEntity::AddComponent<CameraComponent>();
	template MeshComponent& SceneEntity::AddComponent<MeshComponent>();

	template TagComponent& SceneEntity::GetComponent<TagComponent>() const;
	template TransformComponent& SceneEntity::GetComponent<TransformComponent>() const;
	template CameraComponent& SceneEntity::GetComponent<CameraComponent>() const;
	template MeshComponent& SceneEntity::GetComponent<MeshComponent>() const;

	template bool SceneEntity::HasComponent<TagComponent>() const;
	template bool SceneEntity::HasComponent<TransformComponent>() const;
	template bool SceneEntity::HasComponent<CameraComponent>() const;
	template bool SceneEntity::HasComponent<MeshComponent>() const;
}
