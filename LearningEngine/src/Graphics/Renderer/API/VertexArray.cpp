#include "VertexArray.h"

#include "RendererAPI.h"

//#include "Renderer/Platform/D3D11/D3D11VertexArray.h"
#include "Platform/OpenGL/OGLVertexArray.h"

VertexArray* VertexArray::Create()
{
	switch (RendererAPI::GetAPI())
	{
		case RendererAPI::API::OpenGL:
			return new OGLVertexArray();
		default:
			return nullptr;
	}
	return nullptr;
}