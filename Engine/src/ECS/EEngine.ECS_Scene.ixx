export module EEngine.ECS:Scene;
import EEngine.Rendering;
import :Registry;

export namespace EEngine {
	class SceneEntity;

	class Scene {
	public:
		Scene() = default;
		~Scene() = default;

		SceneEntity CreateEntity(const std::string& name = "Empty Entity");
		void DestroyEntity(SceneEntity entity);

		void OnUpdate(Timestep ts);
		void OnRender(Rendering::Renderer& renderer);

		SceneEntity GetPrimaryCameraEntity();

		template<typename T>
		T& Get(uint32_t entity) { return m_Registry.Get<T>(entity); }

		template<typename T1, typename T2, typename... Ts>
		auto Get(uint32_t entity) { return m_Registry.Get<T1, T2, Ts...>(entity); }

		template<typename... Ts>
		auto View() { return m_Registry.View<Ts...>(); }

		template<typename... Ts, typename Func>
		void View(Func&& func) { m_Registry.View<Ts...>(std::forward<Func>(func)); }

		// Mesh asset registration (temporary until a proper AssetRegistry exists)
		uint32_t RegisterMesh(const Shared<Rendering::VertexArray>& vertexArray);
		Shared<Rendering::VertexArray> GetMesh(uint32_t id) const;

	private:
		Registry m_Registry;
		uint32_t m_EntityCount = 0;
		uint32_t m_NextMeshID = 0;
		// ER TODO rename to m_MeshByID
		std::unordered_map<uint32_t, Shared<Rendering::VertexArray>> m_MeshLookup;
		friend class SceneEntity;
	};
}