
#include "OGLRendererAPI.h"

#include <glew.h>

#include "OGLContext.h"

OGLRendererAPI::OGLRendererAPI(WindowsWindow& window)
	: m_Window(window)
{
	m_Context = new OGLContext(window);

}

OGLRendererAPI::~OGLRendererAPI()
{
}

RenderContext* OGLRendererAPI::GetContext()
{
	return m_Context;
}

std::string OGLRendererAPI::GetVersionString()
{
	return std::string((const char*)glGetString(GL_VERSION));
}

void OGLRendererAPI::Clear(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRendererAPI::Resize(const int width, const int height)
{
	m_Viewport = glm::vec4(0, 0, width, height);

	glViewport(0, 0, width, height);
}

void OGLRendererAPI::Present()
{
}

void OGLRendererAPI::UseDepthTesting(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void OGLRendererAPI::EnableBlending(bool enable)
{
	if (enable)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{ 
		glDisable(GL_BLEND);
	}
}

void OGLRendererAPI::BindViewport()
{
}

void OGLRendererAPI::SetViewportSize(int width, int height)
{
	m_Viewport.b = width;
	m_Viewport.a = height;

	glViewport(m_Viewport.r, m_Viewport.g, m_Viewport.b, m_Viewport.a);
}

glm::vec2 OGLRendererAPI::GetViewportSize()
{
	return glm::ivec2(m_Viewport.b, m_Viewport.a);
}

void OGLRendererAPI::SetViewportPosition(int x, int y)
{
	m_Viewport.r = x;
	m_Viewport.g = y;

	glViewport(m_Viewport.r, m_Viewport.g, m_Viewport.b, m_Viewport.a);
}

glm::vec2 OGLRendererAPI::GetViewportPosition()
{
	return glm::ivec2(m_Viewport.r, m_Viewport.g);
}
