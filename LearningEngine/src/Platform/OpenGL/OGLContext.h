#pragma once

#include <string>

#include "Graphics/Renderer/API/RenderContext.h"

class OGLContext : public RenderContext
{
public:
	OGLContext(WindowsWindow& Window);
	~OGLContext();

	virtual void Present() override;

private:
	WindowsWindow m_Window;
};

