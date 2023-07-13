#include "CubeMap.h"

#include "Renderer.h"	

#include "Platform/OpenGL/OGLCubeMap.h"

CubeMap* CubeMap::Create(std::filesystem::path path, CubeMapLayout layout)
{
	switch (Renderer::GetAPI())
	{
	case Renderer::API::OpenGL:
		return new OGLCubeMap(path, layout);
	}
	return nullptr;
}