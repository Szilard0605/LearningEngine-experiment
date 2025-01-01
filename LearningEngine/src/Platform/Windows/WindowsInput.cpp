#include "Core/input.h"

//#include <glew.h>
#include <glfw3.h>

#include "Core/Application.h"
#include "Platform/Windows/WindowsWindow.h"


static uint32_t s_MButtonStates[MAX_BUTTONS];
static uint32_t s_KeyStates[MAX_KEYS];
static glm::vec2 m_MousePosition;
static double scroll_xOffset;
static double scroll_yOffset;
static bool mouse_scrolled = false;


void Input::OnKeyChange(const int key, const int scancode, const int action, const int mods)
{
	SetKeyState(key, action);
}

void Input::SetKeyState(const uint32_t key, const uint32_t state)
{
	s_KeyStates[key] = state;
}

bool Input::IsKeyPressed(uint32_t keycode)
{
	return s_KeyStates[keycode];
}

KeyState Input::GetKeyState(uint32_t keycode)
{
	return (KeyState)s_KeyStates[keycode];
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

void Input::DisableCursor(bool disable)
{
	WindowsWindow* wnd = Application::GetInstance()->GetWindow();
	wnd->DisableCursor(disable);
}

void Input::SetCursorPosition(glm::vec2 position)
{
	WindowsWindow* wnd = Application::GetInstance()->GetWindow();
	wnd->SetCursorPosition(position);
}

KeyState Input::GetMouseButtonState(const uint32_t button)
{
	return (KeyState)s_MButtonStates[button];
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

void Input::SetButtonState(const uint32_t button, const uint32_t state)
{
	s_MButtonStates[button] = state;
}

bool Input::IsMouseButtonPressed(const uint32_t button)
{
	return (s_MButtonStates[button] == GLFW_PRESS);
}

bool Input::IsMouseButtonReleased(const uint32_t button)
{
	return (s_MButtonStates[button] == GLFW_RELEASE);
}

bool Input::IsMouseButtonHold(const uint32_t button)
{
	//TODO
	return (s_MButtonStates[button] != GLFW_RELEASE);
}
