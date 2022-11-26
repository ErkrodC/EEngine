#pragma once

#include "Input.hpp"

namespace EEngine {
	class WindowsInput : public Input {
	protected:
		bool IsKeyPressedImpl(KeyCode keyCode) override;
		bool IsMouseButtonPressedImpl(MouseButtonCode mouseButtonCode) override;
		std::pair<float, float> GetMousePositionImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};
} // EEngine
