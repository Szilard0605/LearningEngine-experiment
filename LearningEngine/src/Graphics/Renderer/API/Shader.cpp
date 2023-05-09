#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OGLShader.h"

Shader* Shader::Create(const std::string& path)
{
	switch (Renderer::GetAPI())
	{
		case Renderer::API::OpenGL:	
			return new OGLShader(path);
		default:
			return nullptr;
	}
	return nullptr;
}

Shader* Shader::Create(const std::string vertex, const std::string fragment)
{
	switch (Renderer::GetAPI())
	{
		case Renderer::API::OpenGL:
			return new OGLShader(vertex, fragment);
		default:
			return nullptr;
	}
	return nullptr;
}
