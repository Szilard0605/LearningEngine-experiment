#include "IndexBuffer.h"

#include "Renderer.h"

//#include "Renderer/Platform/D3D11/D3D11IndexBuffer.h"

#include "Platform/OpenGL/OGLIndexBuffer.h"

IndexBuffer* IndexBuffer::Create(uint32_t* data, uint32_t count)
{
	switch (Renderer::GetAPI())
	{
		case Renderer::API::OpenGL:
			return new OGLIndexBuffer(data, count);
		default:
			return nullptr;
	}
	return nullptr;
}