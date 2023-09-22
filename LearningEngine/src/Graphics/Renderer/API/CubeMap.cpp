#include "CubeMap.h"

#include "RendererAPI.h"	

#include "Platform/OpenGL/OGLCubeMap.h"

CubeMap* CubeMap::Create(std::filesystem::path path, CubeMapLayout layout)
{
	switch (RendererAPI::GetAPI())
	{
	case RendererAPI::API::OpenGL:
		return new OGLCubeMap(path, layout);
	}
	return nullptr;
}