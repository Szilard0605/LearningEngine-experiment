#pragma once

#include <functional>


#include <cstdint>
#include <glm.hpp>
#include "KeyCodes.h"

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

struct GLFWwindow;

enum KeyAction
{
	PRESSED = 1,
	RELEASED = 0,
	REPEAT = 2
};

class Input
{
private:

	Input() {}
	~Input() {}

public:

	//Keyboard
	static void OnKeyChange(const int key, const int scancode, const int action, const int mods);
	static void SetKeyState(const uint32_t key, const bool state);
	static bool IsKeyPressed(uint32_t keycode);

	//Mouse
	static void OnMouseMove(const double mx, const double my);
	static void OnMouseChange(const int button, const int action, const int mods);
	static void OnMouseScrolled(const double xoffset, const double yoffset);

	//static bool MouseScrolled(); // TEMPORARY

	static glm::vec2 GetScrollOffset();
	static glm::vec2 GetMousePosition();
	static void SetMousePosition(glm::vec2 position);
	static void SetButtonState(const unsigned int button, const int state);
	static bool IsMouseButtonPressed(const unsigned int button);
	static bool IsMouseButtonReleased(const unsigned int button);
	static bool IsMouseButtonHold(const unsigned int button);

	static void ShowCursor(bool show = true);
	
};

