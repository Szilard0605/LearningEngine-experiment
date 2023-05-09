#include "RenderContext.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Platform/OpenGL/OGLContext.h"

//#include "Renderer/Platform/D3D11/D3D11Context.h"

RenderContext* RenderContext::Create(WindowsWindow& window)
{
	switch (Renderer::GetAPI())
	{
		case Renderer::API::OpenGL:
			return new OGLContext(window);
		default:
			return nullptr;
	}
	return nullptr;
}