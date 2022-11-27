#pragma once

#include <imgui.h>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include "Layer.hpp"

namespace EEngine {
	class IMGUILayer : public Layer {
	public:
		IMGUILayer();
		~IMGUILayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnIMGUIRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
} // EEngine
