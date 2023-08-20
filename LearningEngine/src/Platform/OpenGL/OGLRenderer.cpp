
#include "OGLRenderer.h"

#include <glew.h>

#include "OGLContext.h"

OGLRenderer::OGLRenderer(WindowsWindow& window)
	: m_Window(window)
{
	m_Context = new OGLContext(window);

}

OGLRenderer::~OGLRenderer()
{
}

RenderContext* OGLRenderer::GetContext()
{
	return m_Context;
}

std::string OGLRenderer::GetVersionString()
{
	return std::string((const char*)glGetString(GL_VERSION));
}

void OGLRenderer::Clear(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderer::Resize(const int width, const int height)
{
	m_Viewport = glm::vec4(0, 0, width, height);

	glViewport(0, 0, width, height);
}

void OGLRenderer::Present()
{
}

void OGLRenderer::UseDepthTesting(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void OGLRenderer::BindViewport()
{
}

void OGLRenderer::SetViewportSize(int width, int height)
{
	m_Viewport.b = width;
	m_Viewport.a = height;

	glViewport(m_Viewport.r, m_Viewport.g, m_Viewport.b, m_Viewport.a);
}

glm::vec2 OGLRenderer::GetViewportSize()
{
	return glm::ivec2(m_Viewport.b, m_Viewport.a);
}

void OGLRenderer::SetViewportPosition(int x, int y)
{
	m_Viewport.r = x;
	m_Viewport.g = y;

	glViewport(m_Viewport.r, m_Viewport.g, m_Viewport.b, m_Viewport.a);
}

glm::vec2 OGLRenderer::GetViewportPosition()
{
	return glm::ivec2(m_Viewport.r, m_Viewport.g);
}
