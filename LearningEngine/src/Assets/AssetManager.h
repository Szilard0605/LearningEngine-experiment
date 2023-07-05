#pragma once

#include "Asset.h"

#include <map>
#include <filesystem>

class AssetManager
{
public:
	static AssetHandle CreateAsset(AssetData data);
	static AssetHandle CreateAsset(AssetType type);
	static AssetHandle CreateAssetFromFile(std::filesystem::path path);
	static void LoadAssetsFromRegistry(std::filesystem::path path);
	static void SaveAsset(AssetHandle handle, std::filesystem::path path);
	static AssetType GetAssetTypeFromFileExtension(std::filesystem::path path);
	static Asset& GetAssetByHandle(AssetHandle handle);
};

