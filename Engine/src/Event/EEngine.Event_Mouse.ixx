export module EEngine.Event:Mouse;
import EEngine.Core;
import EEngine.Standard;
import :Base;
#include "Event.hpp"

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Mouse Events
	// ============================================================================
	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float_t x, float_t y)
			: m_MouseX(x), m_MouseY(y) {}

		inline float_t GetX() const { return m_MouseX; }
		inline float_t GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float_t m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float_t xOffset, float_t yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float_t GetXOffset() const { return m_XOffset; }
		inline float_t GetYOffset() const { return m_YOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float_t m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		inline MouseButtonCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		explicit MouseButtonEvent(const MouseButtonCode& button)
			: m_Button(button) {}

		MouseButtonCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		explicit MouseButtonPressedEvent(MouseButtonCode button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << (int)m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		explicit MouseButtonReleasedEvent(MouseButtonCode button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << (int)m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
