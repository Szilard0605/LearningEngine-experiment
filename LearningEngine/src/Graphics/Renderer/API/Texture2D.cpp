#include "Texture2D.h"

#include "Renderer.h"

#include "Platform/OpenGL/OGLTexture2D.h"

Texture2D* Texture2D::Create(const std::string path)
{
	switch (Renderer::GetAPI())
	{
		case Renderer::API::OpenGL:
			return new OGLTexture2D(path);
		default:
			return nullptr;
	}
	return nullptr;
}

Texture2D* Texture2D::Create(uint32_t width, uint32_t height)
{
	switch (Renderer::GetAPI())
	{
		case Renderer::API::OpenGL:
			return new OGLTexture2D(width, height);
		default:
			return nullptr;
	}
	return nullptr;
}

Texture2D* Texture2D::Create(Texture2DProperties& properties)
{
	switch (Renderer::GetAPI())
	{
	case Renderer::API::OpenGL:
		return new OGLTexture2D(properties);
	default:
		return nullptr;
	}
	return nullptr;
}

Texture2D* Texture2D::Create(Image& image)
{
	switch (Renderer::GetAPI())
	{
	case Renderer::API::OpenGL:
		return new OGLTexture2D(image);
	default:
		return nullptr;
	}
	return nullptr;
}
