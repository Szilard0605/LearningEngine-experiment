
#include "RendererAPI.h"

#include "Platform/OpenGL/OGLRendererAPI.h"
#include "RenderContext.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

RendererAPI* RendererAPI::Create(WindowsWindow& window)
{
	switch (s_API)
	{
		case RendererAPI::API::OpenGL:
			return new OGLRendererAPI(window);
		default:
			return nullptr;
	}
	return nullptr;
}