#pragma once

#include "Asset.h"
#include "Graphics/Renderer/Image.h"

class ImageAsset : public Asset
{
public:
	ImageAsset();
	ImageAsset(Image& image);
	ImageAsset(std::string path);

	AssetType GetType() const override { return AssetType::Image; }

	const std::string& GetFilePath() { return m_Image.GetFilePath(); }
	Image& GetImage() { return m_Image; }
	void SetImage(Image& image) { m_Image = image; }

private:
	std::string m_FilePath;
	Image m_Image;
};

