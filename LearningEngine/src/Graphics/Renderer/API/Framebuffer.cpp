#include "Framebuffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OGLFramebuffer.h"

Framebuffer* Framebuffer::Create(FramebufferSpecifications& specs)
{
    switch (Renderer::GetAPI())
    {
        case Renderer::API::OpenGL:   
            return new OGLFramebuffer(specs);
        default:
            return nullptr;
    }
    return nullptr;
}
