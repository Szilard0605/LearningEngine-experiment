#pragma once

enum class EventType
{
	None = 0,
	KEY_CHANGE,
	MOUSE_BUTTON_CHANGE, MOUSE_IS_MOVED, MOUSE_SCROLLED
};

class Event
{
	public:
		bool Handled = false;

		virtual ~Event() = default;

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (GetType() == T::GetStaticType())
			{
				Handled |= func(static_cast<T&>(*this));
				return true;
			}
			return false;
		}

	
		virtual EventType GetType() const = 0;
};
