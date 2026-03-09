export module EEngine.ECS:Registry;
import EEngine.Core;
import EEngine.Standard;
import :ComponentPool;

export namespace EEngine {
	class Registry {
	public:
		template<typename T, typename... Args>
		T& Add(uint32_t entity, Args&&... args) {
			auto* pool = GetPool<T>();
			return pool->Add(entity, T(std::forward<Args>(args)...));
		}

		template<typename T>
		T& Get(uint32_t entity) { return GetPool<T>()->Get(entity); }

		template<typename T1, typename T2, typename... Ts>
		auto Get(uint32_t entity) {
			return std::tie(GetPool<T1>()->Get(entity), GetPool<T2>()->Get(entity), GetPool<Ts>()->Get(entity)...);
		}

		template<typename T>
		bool Has(uint32_t entity) const {
			auto it = m_Pools.find(typeid(T));
			if (it == m_Pools.end()) { return false; }
			return it->second->Has(entity);
		}

		template<typename... Ts>
		std::vector<uint32_t> View() {
			// Gather all relevant pools, find the smallest to lead iteration
			IComponentPool* pools[] = { static_cast<IComponentPool*>(GetPool<Ts>())... };
			IComponentPool* lead = *std::min_element(std::begin(pools), std::end(pools),
				[](const IComponentPool* a, const IComponentPool* b) {
					return a->Count() < b->Count();
				}
			);

			std::vector<uint32_t> result;
			for (uint32_t entity : lead->GetDenseEntities()) {
				if ((GetPool<Ts>()->Has(entity) && ...)) {
					result.push_back(entity);
				}
			}

			return result;
		}

		template<typename... Ts, typename Func>
		void View(Func&& func) {
			for (uint32_t entity : View<Ts...>()) {
				std::apply(func, std::tie(GetPool<Ts>()->Get(entity)...));
			}
		}


		void Remove(uint32_t entity) {
			for (auto& [type, pool] : m_Pools) {
				pool->Remove(entity);
			}
		}

	private:
		std::unordered_map<std::type_index, Unique<IComponentPool>> m_Pools;

		template<typename T>
		ComponentPool<T>* GetPool() {
			auto& pool = m_Pools[typeid(T)];
			if (!pool) { pool = MakeUnique<ComponentPool<T>>(); }

			return static_cast<ComponentPool<T>*>(pool.get());
		}
	};
}