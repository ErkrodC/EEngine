export module EEngine.Event:Keyboard;
import EEngine.Core;
import EEngine.Standard;
import :Base;
#include "Event.hpp"

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Keyboard Events
	// ============================================================================

	class KeyEvent : public Event {
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		explicit KeyEvent(KeyCode keyCode)
			: m_KeyCode(keyCode) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(KeyCode keyCode, int32_t repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		inline int32_t GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << static_cast<int>(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		};

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int32_t m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		explicit KeyReleasedEvent(KeyCode keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << static_cast<int>(m_KeyCode);
			return ss.str();
		};

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public Event {
	public:
		explicit KeyTypedEvent(uint32_t unicode)
			: m_Unicode(unicode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_Unicode;
			return ss.str();
		}

		uint32_t GetUnicode() const { return m_Unicode; }

		EVENT_CLASS_TYPE(KeyTyped)
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	private:
		uint32_t m_Unicode;
	};
}
