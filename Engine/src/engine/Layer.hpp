#pragma once

#include <Core/Timestep.hpp>
#include "engine/Core.hpp"
#include "engine/Events/Event.hpp"

namespace EEngine {
	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

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
