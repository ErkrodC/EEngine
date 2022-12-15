export module EEngine.Application:Layer;
import EEngine.Core;
import EEngine.Event;
import EEngine.std.core;

export namespace EEngine {
	class Layer {
	public:
		Layer(const std::string& name = "Layer") {}
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
} // EEngine