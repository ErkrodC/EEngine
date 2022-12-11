module;
#include <utility>

export module EEngine.Application:WindowsInput;
import EEngine.Core;

export namespace EEngine {
	class WindowsInput : public IInput {
	protected:
		bool IsKeyPressedImpl(KeyCode keyCode) override;
		bool IsMouseButtonPressedImpl(MouseButtonCode mouseButtonCode) override;
		std::pair<float, float> GetMousePositionImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};
} // EEngine
