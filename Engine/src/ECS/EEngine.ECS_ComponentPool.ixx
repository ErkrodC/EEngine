export module EEngine.ECS:ComponentPool;
import EEngine.Standard;

export namespace EEngine {
	constexpr uint32_t NULL_INDEX = 0xFFFF'FFFF;

	class IComponentPool {
	public:
		virtual ~IComponentPool() = default;
		virtual bool Has(uint32_t entity) const = 0;
		virtual void Remove(uint32_t entity) = 0;
		virtual uint32_t Count() const = 0;
		virtual const std::vector<uint32_t>& GetDenseEntities() const = 0;
	};

	template<typename T>
	class ComponentPool : public IComponentPool {
	public:
		T& Add(uint32_t entity, T&& component) {
			if (Has(entity)) { return m_Components[m_Sparse[entity]]; }
			// Map Entity ID to the end of dense array
			m_Sparse.resize(std::max(static_cast<size_t>(entity) + 1, m_Sparse.size()), NULL_INDEX);
			m_Sparse[entity] = static_cast<uint32_t>(m_Components.size());

			m_Dense.push_back(entity);
			m_Components.push_back(std::move(component));
			return m_Components.back();
		}

		T& Get(uint32_t entity) { return m_Components[m_Sparse[entity]]; }

		bool Has(uint32_t entity) const override {
			return entity < m_Sparse.size() && m_Sparse[entity] != NULL_INDEX;
		}

		void Remove(uint32_t entity) override {
			if (!Has(entity)) { return; }

			uint32_t componentIdxToRemove = m_Sparse[entity];
			m_Components[componentIdxToRemove] = std::move(m_Components.back());
			m_Components.pop_back();
			m_Sparse[entity] = NULL_INDEX;

			uint32_t entityIdToUpdate = m_Dense.back();
			m_Sparse[entityIdToUpdate] = componentIdxToRemove;
			m_Dense[componentIdxToRemove] = entityIdToUpdate;
			m_Dense.pop_back();
		}

		auto& GetComponents() { return m_Components; }
		const std::vector<uint32_t>& GetDenseEntities() const override { return m_Dense; }
		uint32_t Count() const override { return static_cast<uint32_t>(m_Components.size()); }

	private:
		std::vector<uint32_t> m_Sparse; // Entity ID -> Index (into m_Components)
		std::vector<uint32_t> m_Dense; // Index (into m_Components) -> Entity ID
		std::vector<T> m_Components;
	};
}