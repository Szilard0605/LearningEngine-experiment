
#include "Renderer.h"

#include "Platform/OpenGL/OGLRenderer.h"
#include "RenderContext.h"

Renderer::API Renderer::s_API = Renderer::API::OpenGL;

Renderer* Renderer::Create(WindowsWindow& window)
{
	switch (s_API)
	{
		case Renderer::API::OpenGL: 
			return new OGLRenderer(window);
		default:
			return nullptr;
	}
	return nullptr;
}