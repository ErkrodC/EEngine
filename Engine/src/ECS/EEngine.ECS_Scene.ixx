export module EEngine.ECS:Scene;
import EEngine.Rendering;
import :Entity;
import :Registry;

export namespace EEngine {
	class Scene {
	public:
		Scene() = default;
		~Scene() = default;

		SceneEntity CreateEntity(const std::string& name = "Empty Enitty");
		void DestroyEntity(SceneEntity entity);

		void OnUpdate(Timestep ts);
		void OnRender(Rendering::Renderer& renderer);;

		SceneEntity GetPrimaryCameraEntity();

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