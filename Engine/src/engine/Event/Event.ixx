export module EEngine.Event:Event;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;

export namespace EEngine {
	// ER TODO Events in EEngine are currently synchronous
	//  make async, likely using event queue, and processing during event stage in update loop

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = Math::Bit(0),
		EventCategoryInput = Math::Bit(1),
		EventCategoryKeyboard = Math::Bit(2),
		EventCategoryMouse = Math::Bit(3),
		EventCategoryMouseButton = Math::Bit(4)
	};

	class Event {
		friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) const {
			return GetCategoryFlags() & category;
		}

		bool Handled = false;
	};

	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		explicit EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}