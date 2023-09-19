#include "VertexBuffer.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OGLVertexBuffer.h"

VertexBuffer* VertexBuffer::Create(uint32_t size)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::OpenGL:
            return new OGLVertexBuffer(size);
        default:
            return nullptr;
    }
    return nullptr;
}



VertexBuffer* VertexBuffer::Create(const void* data, uint32_t size)
{
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return new OGLVertexBuffer(data, size);
    default:
        return nullptr;
    }
    return nullptr;
}
