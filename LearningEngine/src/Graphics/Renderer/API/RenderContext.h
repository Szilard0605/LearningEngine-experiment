#pragma once

//#include "Core/Window.h"

#include "RendererAPI.h"

#include "Platform/Windows/WindowsWindow.h"

class RenderContext
{
	public:
		static RenderContext* Create(WindowsWindow& window);

		virtual void Present() = 0;
};

