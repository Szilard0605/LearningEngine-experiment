#pragma once

#include "Graphics/Renderer/API/CubeMap.h"

#include <map>

class OGLCubeMap : public CubeMap
{
public:
	OGLCubeMap(std::filesystem::path path, CubeMapLayout layout);

	virtual void SetFaceImage(CubeMapFace face, Image& image) override;
	//virtual Image GetFaceImage(CubeMapFace face) override;
	virtual void Bind() override;
	virtual void Unbind() override;

	void LoadCubeMapFaceLayout(Image& image, CubeMapLayout layout);

private:
	uint32_t m_CubeMap;
	
	//Need to change it to store images with our Image API
	std::map<CubeMapFace, std::vector<unsigned char>> m_FaceMap;
};

