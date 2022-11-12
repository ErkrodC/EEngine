#pragma once

#include <Events/KeyEvent.hpp>
#include "Layer.hpp"

namespace EEngine {
	class IMGUILayer : public Layer {
	public:
		IMGUILayer();
		~IMGUILayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(EEngine::Event &event) override;

	private:
		float m_Time = 0.0f;
		bool OnKeyPressed(EEngine::KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
	};
} // EEngine
