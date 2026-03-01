export module EEngine.Application:Input;
import EEngine.Core;
import EEngine.Standard;
import :Window;

using namespace EEngine;

namespace EEngine {
	// ============================================================================
	// Windows Input Implementation
	// ============================================================================
	export class WindowsInput {
	public:
		WindowsInput(Shared<Window> window) : m_Window(std::move(window)) { }

		bool IsKeyPressed(KeyCode keyCode) const;

		bool IsMouseButtonPressed(MouseButtonCode mouseButtonCode) const;

		std::pair<float_t, float_t> GetMousePosition() const;

		float_t GetMouseX() const {
			auto[x, y] = GetMousePosition();
			return x;
		}

		float_t GetMouseY() const {
			auto[x, y] = GetMousePosition();
			return y;
		}

	private:
		Shared<Window> m_Window;
	};

	// ============================================================================
	// Input Alias, Concept, and Static Assert
	// ============================================================================
	class MacInput; class LinuxInput;
	export using Input = std::conditional_t<g_Platform == Platform::MacOS,
		MacInput,
		std::conditional_t<g_Platform == Platform::Linux,
			LinuxInput,
			WindowsInput
		>
	>;

	export template<typename TInput>
	concept InputConcept = requires(
		TInput input,
		Shared<Window> window,
		KeyCode keyCode,
		MouseButtonCode button
	) {
		{ TInput(window) } -> std::same_as<TInput>;
		{ input.IsKeyPressed(keyCode) } -> std::same_as<bool>;
		{ input.IsMouseButtonPressed(button) } -> std::same_as<bool>;
		{ input.GetMouseX() } -> std::same_as<float_t>;
		{ input.GetMouseY() } -> std::same_as<float_t>;
		{ input.GetMousePosition() } -> std::same_as<std::pair<float_t, float_t>>;
	};

	static_assert(InputConcept<Input>);
}
