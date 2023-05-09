#pragma once

#include "Platform/Windows/WindowsWindow.h"

class ImGuiCore
{
public:
	static void Init(WindowsWindow& window);
	static void Shutdown();
	static void BeginFrame();
	static void EndFrame();
};

