export module EEngine.ECS:Entity;
import EEngine.Standard;
import :Scene;

export namespace EEngine {
	class SceneEntity {
	public:
		SceneEntity() = default;
		SceneEntity(uint32_t handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_Scene->m_Registry.Add<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() const {
			return m_Scene->m_Registry.Get<T>(m_EntityHandle);
		}

		template<typename T1, typename T2, typename... Ts>
		auto GetComponent() const {
			return m_Scene->m_Registry.Get<T1, T2, Ts...>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() const {
			return m_Scene->m_Registry.Has<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != 0 && m_Scene != nullptr; }
		operator uint32_t() const { return m_EntityHandle; }
		bool operator==(const SceneEntity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }

	private:
		uint32_t m_EntityHandle{ 0 };
		Scene* m_Scene{ nullptr };
	};
}