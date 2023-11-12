#include "ShaderBuffer.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OGLShaderBuffer.h"

ShaderBuffer* ShaderBuffer::Create(__int64 size, ShaderType type)
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
