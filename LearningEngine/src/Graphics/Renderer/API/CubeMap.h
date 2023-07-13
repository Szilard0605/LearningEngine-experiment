#pragma once

#include "Graphics/Renderer/Image.h"

#include <stdint.h>
#include <filesystem>
#include <map>

enum class CubeMapFace : int
{
	Left = 0,
	Right = 2,
	Top = 4,
	Bottom = 5,
	Front = 1,
	Back = 3

};

enum class CubeMapLayout
{
	HorizontalCross,
	VerticalCross
};

class CubeMap
{
public:
	static CubeMap* Create(std::filesystem::path path, CubeMapLayout layout);
	//static CubeMap* Create();

	virtual void SetFaceImage(CubeMapFace face, Image& image) = 0;
	//virtual Image GetFaceImage(CubeMapFace face) = 0;
	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	//virtual std::map<CubeMapFace, Image> GetFaces() = 0;
};

