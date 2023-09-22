#include "ShaderBuffer.h"

#include "RendererAPI.h"
//#include "Renderer/Platform/D3D11/D3D11ConstantBuffer.h"

ShaderBuffer* ShaderBuffer::Create(const void* data, uint32_t size, uint32_t slot, Type type)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::OpenGL:
            //return new D3D11ConstantBuffer(data, size, slot, type);
            return nullptr;
        default:
            return nullptr;
    }

    return nullptr;
}
