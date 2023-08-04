#include "Core/input.h"

//#include <glew.h>
#include <glfw3.h>

#include "Core/Application.h"
#include "Platform/Windows/WindowsWindow.h"


static int m_buttons[MAX_BUTTONS];
static bool m_KeyState[MAX_KEYS];
static glm::vec2 m_MousePosition;
static double scroll_xOffset;
static double scroll_yOffset;
static bool mouse_scrolled = false;


void Input::OnKeyChange(const int key, const int scancode, const int action, const int mods)
{
	SetKeyState(key, action != GLFW_RELEASE);
}

void Input::SetKeyState(const uint32_t key, const bool state)
{
	m_KeyState[key] = state;
}

bool Input::IsKeyPressed(uint32_t keycode)
{
	return m_KeyState[keycode];
}

void Input::OnMouseMove(const double mx, const double my)
{
	SetMousePosition(glm::vec2(mx, my));
}

void Input::OnMouseChange(const int button, const int action, const int mods)
{
	SetButtonState(button, action);
}

void Input::OnMouseScrolled(const double xoffset, const double yoffset)
{

	scroll_xOffset = xoffset;
	scroll_yOffset = yoffset;

}

void Input::ShowCursor(bool show)
{
	WindowsWindow* wnd = Application::GetInstance()->GetWindow();
	wnd->ShowCursor(show);
}

glm::vec2 Input::GetScrollOffset()
{
	return glm::vec2(scroll_xOffset, scroll_yOffset);
}

glm::vec2 Input::GetMousePosition()
{
	return m_MousePosition;
}

void Input::SetMousePosition(glm::vec2 position)
{
	m_MousePosition = position;
}

void Input::SetButtonState(const unsigned int button, const int state)
{
	m_buttons[button] = state;
}

bool Input::IsMouseButtonPressed(const unsigned int button)
{
	return (m_buttons[button] == GLFW_PRESS);
}

bool Input::IsMouseButtonReleased(const unsigned int button)
{
	return (m_buttons[button] == GLFW_RELEASE);
}

bool Input::IsMouseButtonHold(const unsigned int button)
{
	//TODO
	return false;
}
