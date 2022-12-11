module;
#include <utility>

export module EEngine.Core:IInput;
import :KeyCode;
import :MouseButtonCode;

export namespace EEngine {
	class IInput {
	public:
		virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseButtonCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	};
}