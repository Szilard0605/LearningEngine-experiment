#include "Texture2D.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OGLTexture2D.h"

Texture2D* Texture2D::Create(const std::string path)
{
	switch (RendererAPI::GetAPI())
	{
		case RendererAPI::API::OpenGL:
			return new OGLTexture2D(path);
		default:
			return nullptr;
	}
	return nullptr;
}

Texture2D* Texture2D::Create(uint32_t width, uint32_t height)
{
	switch (RendererAPI::GetAPI())
	{
		case RendererAPI::API::OpenGL:
			return new OGLTexture2D(width, height);
		default:
			return nullptr;
	}
	return nullptr;
}

Texture2D* Texture2D::Create(Texture2DProperties& properties)
{
	switch (RendererAPI::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return new OGLTexture2D(properties);
	default:
		return nullptr;
	}
	return nullptr;
}

Texture2D* Texture2D::Create(Image& image)
{
	switch (RendererAPI::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return new OGLTexture2D(image);
	default:
		return nullptr;
	}
	return nullptr;
}
