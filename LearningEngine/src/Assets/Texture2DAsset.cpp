#include "Texture2DAsset.h"

Texture2DAsset::Texture2DAsset(Texture2D& texture)
	: m_Texture(&texture)
{
}

Texture2DAsset::Texture2DAsset(std::string& path)
{
	m_Texture = Texture2D::Create(path);
}

Texture2DAsset::Texture2DAsset(uint32_t width, uint32_t height)
{
	m_Texture = Texture2D::Create(width, height);
}
