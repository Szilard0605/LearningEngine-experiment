#include "Framebuffer.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OGLFramebuffer.h"

Framebuffer* Framebuffer::Create(FramebufferSpecifications specs)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::API::OpenGL:
            return new OGLFramebuffer(specs);
        default:
            return nullptr;
    }
    return nullptr;
}
