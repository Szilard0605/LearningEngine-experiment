#include "VertexArray.h"

#include "Renderer.h"

//#include "Renderer/Platform/D3D11/D3D11VertexArray.h"
#include "Platform/OpenGL/OGLVertexArray.h"

VertexArray* VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
		case Renderer::API::OpenGL:
			return new OGLVertexArray();
		default:
			return nullptr;
	}
	return nullptr;
}