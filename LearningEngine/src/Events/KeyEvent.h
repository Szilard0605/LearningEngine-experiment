#pragma once

#include "Event.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

class KeyEvent : public Event
{
	protected:
		int m_KeyCode;
		int m_Action;
	public:
		KeyEvent(int keycode, int action)
			: m_KeyCode(keycode), m_Action(action)
		{

		}

		EventType GetType() const override { return EventType::KEY_CHANGE; }
		static EventType GetStaticType() { return EventType::KEY_CHANGE; }
		int GetKeyCode() const { return m_KeyCode; }
		int GetAction() const { return m_Action; }
};