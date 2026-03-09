export module EEngine.ECS:Entity;
import EEngine.Standard;

export namespace EEngine {
	class Scene;

	class SceneEntity {
	public:
		SceneEntity() = default;
		SceneEntity(uint32_t handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		T& GetComponent() const;

		template<typename T1, typename T2, typename... Ts>
		auto GetComponent() const;

		template<typename T>
		bool HasComponent() const;

		operator bool() const { return m_EntityHandle != 0 && m_Scene != nullptr; }
		operator uint32_t() const { return m_EntityHandle; }
		bool operator==(const SceneEntity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }

	private:
		uint32_t m_EntityHandle{ 0 };
		Scene* m_Scene{ nullptr };
	};
}