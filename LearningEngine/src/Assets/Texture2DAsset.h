#pragma once

#include "Asset.h"
#include "Graphics/Renderer/API/Texture2D.h"

class Texture2DAsset : public Asset
{
public:
	Texture2DAsset() = default;
	Texture2DAsset(Texture2D& texture);
	Texture2DAsset(std::string& path);
	Texture2DAsset(uint32_t width, uint32_t height);

	AssetType GetType() const override { return AssetType::Texture2D; }
	const Texture2D* GetTexture() { return m_Texture; }

private:
	Texture2D* m_Texture;
};

