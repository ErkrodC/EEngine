export module EEngine.Application:LayerStack;
import :Layer;
import EEngine.Standard;

export namespace EEngine {
	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack() {
			for (Layer* layer : m_Layers) {
				delete layer;
			}
		}

		void PushLayer(Layer* layer) {
			m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
			++m_LayerInsertIndex;
		}

		void PushOverlay(Layer* overlay) {
			m_Layers.emplace_back(overlay);
		}

		void PopLayer(Layer* layer) {
			auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
			if (it != m_Layers.end()) {
				m_Layers.erase(it);
				--m_LayerInsertIndex;
			}
		}

		void PopOverlay(Layer* overlay) {
			auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
			if (it != m_Layers.end()) {
				m_Layers.erase(it);
			}
		}

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
} // EEngine
