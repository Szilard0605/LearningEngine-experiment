#pragma once

#include "Asset.h"

#include <filesystem>

class AssetRegistry
{
public:
	//static void Serialize(Asset asset, std::filesystem::path path);
	//static Asset Deserialize(std::filesystem::path path);
	static void LoadAssets(std::filesystem::path path);
	static void SaveAsset(AssetHandle handle, std::filesystem::path path);
};

