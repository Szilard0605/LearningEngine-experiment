#pragma once

#include <string>

enum class AssetType
{
	Image, Texture2D
};

class Asset
{
public:
	~Asset() = default;

	virtual AssetType GetType() const = 0;
};

