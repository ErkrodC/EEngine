export module EEngine.Application:Layer;
import EEngine.Core;
import EEngine.Event;
import EEngine.Standard;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Layer & LayerStack
	// ============================================================================

	class Layer {
	public:
		Layer(const std::string& name = "Layer") : m_DebugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep) {}
		virtual void OnIMGUIRender() {}
		virtual void OnEvent(Event&) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack() = default;

		LayerStack(const LayerStack&) = delete;
		LayerStack& operator=(const LayerStack&) = delete;
		LayerStack(LayerStack&&) noexcept = default;
		LayerStack& operator=(LayerStack&&) noexcept = default;

		void PushLayer(Ref<Layer> layer) {
			m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
			++m_LayerInsertIndex;
		}

		void PushOverlay(Ref<Layer> overlay) {
			m_Layers.emplace_back(std::move(overlay));
		}

		void PopLayer(Layer* layer) {
			const auto it = std::ranges::find_if(
				m_Layers,
				[layer](const Ref<Layer>& ptr) { return ptr.get() == layer; }
			);

			if (it != m_Layers.end()) {
				m_Layers.erase(it);
				--m_LayerInsertIndex;
			}
		}

		void PopOverlay(Layer* overlay) {
			const auto it = std::ranges::find_if(
				m_Layers,
				[overlay](const Ref<Layer>& ptr) { return ptr.get() == overlay; }
			);

			if (it != m_Layers.end()) {
				m_Layers.erase(it);
			}
		}

		class Iterator {
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = Layer*;
			using difference_type = std::ptrdiff_t;
			using pointer = Layer**;
			using reference = Layer*&;

			explicit Iterator(std::vector<Ref<Layer>>::iterator it) : m_It(it) {}

			Layer* operator*() const { return m_It->get(); }
			Layer* operator->() const { return m_It->get(); }
			Iterator& operator++() { ++m_It; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++m_It; return tmp; }
			Iterator& operator--() { --m_It; return *this; }
			Iterator operator--(int) { Iterator tmp = *this; --m_It; return tmp; }
			bool operator==(const Iterator& other) const { return m_It == other.m_It; }
			bool operator!=(const Iterator& other) const { return m_It != other.m_It; }

		private:
			std::vector<Ref<Layer>>::iterator m_It;
		};

		Iterator begin() { return Iterator(m_Layers.begin()); }
		Iterator end() { return Iterator(m_Layers.end()); }

	private:
		std::vector<Ref<Layer>> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}
