#include "ImageAsset.h"

ImageAsset::ImageAsset()
{
}

ImageAsset::ImageAsset(Image& image)
	: m_Image(image)
{
}

ImageAsset::ImageAsset(std::string path)
	: m_FilePath(path)
{
	m_Image = Image(path);
}
