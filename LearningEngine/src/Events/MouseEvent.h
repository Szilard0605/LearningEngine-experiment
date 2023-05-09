#pragma once

#include "Event.h"

#include "glm.hpp"

class MouseButtonEvent : public Event
{
	protected:
		int m_MouseButton;
		int m_Action;
	public:
		MouseButtonEvent(int keycode, int action)
			: m_MouseButton(keycode), m_Action(action)
		{
		}

		EventType GetType() const override { return EventType::MOUSE_BUTTON_CHANGE; }
		static EventType GetStaticType() { return EventType::MOUSE_BUTTON_CHANGE; }
		int GetButton() const { return m_MouseButton; }
		int GetAction() const { return m_Action; }
};

class MouseMoveEvent : public Event
{
	protected:
		glm::vec2 m_Position;
	public:
		MouseMoveEvent(const glm::vec2 position)
			: m_Position(position)
		{
			
		}

		EventType GetType() const override { return EventType::MOUSE_MOVED; }
		static EventType GetStaticType() { return EventType::MOUSE_MOVED; }
		glm::vec2 GetPosition() { return m_Position; }
};

class MouseScrolledEvent : public Event
{
protected:
	glm::vec2 m_Offset;
public:
	MouseScrolledEvent(const glm::vec2 offset)
		: m_Offset(offset)
	{

	}

	EventType GetType() const override { return EventType::MOUSE_SCROLLED; }
	static EventType GetStaticType() { return EventType::MOUSE_SCROLLED; }
	glm::vec2 GetOffset() { return m_Offset; }
};