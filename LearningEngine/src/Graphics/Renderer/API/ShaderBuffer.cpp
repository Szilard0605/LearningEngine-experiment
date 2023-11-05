#include "ShaderBuffer.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OGLShaderBuffer.h"

ShaderBuffer* ShaderBuffer::Create( uint32_t size, ShaderType type)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::OpenGL:
            return new OGLShaderBuffer(size);
        default:
            return nullptr;
    }

    return nullptr;
}
