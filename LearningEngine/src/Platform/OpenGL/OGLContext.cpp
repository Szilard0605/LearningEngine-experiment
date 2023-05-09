#include "OGLContext.h"

#include <glew.h>
#include <glfw3.h>

#include <stdio.h>

#include "Log/Log.h"


OGLContext::OGLContext(WindowsWindow& Window)
	: m_Window(Window)
{
	if (glewInit() != GLEW_OK)
	{
		// Error message here
		LE_LOG_ERROR("Cannot initialize GLEW");
		return;
	}
}

OGLContext::~OGLContext()
{
}

void OGLContext::Present()
{

}
