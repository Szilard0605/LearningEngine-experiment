#include "AssetManager.h"
#include "AssetRegistry.h"

#include "Utils/Random.h"

static std::map<AssetHandle, Asset> s_AssetMap;
static std::map<std::string, AssetType> s_AssetTypeExtensions = 
{
	{".png", AssetType::Texture2D},
	{".jpg", AssetType::Texture2D},
	{".jpeg", AssetType::Texture2D},
	{".lescene", AssetType::Scene}
};

AssetHandle AssetManager::CreateAsset(AssetData data)
{
	AssetHandle handle = Utils::Random::uint32();
	s_AssetMap[handle] = Asset(data);
	return handle;
}

AssetHandle AssetManager::CreateAsset(AssetType type)
{
	AssetHandle handle = Utils::Random::uint32();
	
	AssetData data;
	data.Type = type;

	s_AssetMap[handle] = Asset(data);
	return handle;
}

void AssetManager::LoadAssetsFromRegistry(std::filesystem::path path)
{
	AssetRegistry::LoadAssets(path);
}

void AssetManager::SaveAllAssets(std::filesystem::path path)
{
	for (auto it = s_AssetMap.begin(); it != s_AssetMap.end(); it++)
	{
		AssetRegistry::SaveAsset(it->first, path);
	}
}

AssetHandle AssetManager::CreateAssetFromFile(std::filesystem::path path)
{
	// Deserialize Asset file (.leasset)
	AssetHandle handle = Utils::Random::uint32();

	AssetData data;
	data.Type = GetAssetTypeFromFileExtension(path);
	data.Path = path;

	s_AssetMap[handle] = Asset(data);
	return handle;
}

void AssetManager::SaveAsset(AssetHandle handle, std::filesystem::path path)
{
	AssetRegistry::SaveAsset(handle, path);
}

AssetType AssetManager::GetAssetTypeFromFileExtension(std::filesystem::path path)
{
	return s_AssetTypeExtensions[path.extension().string()];
}

Asset& AssetManager::GetAssetByHandle(AssetHandle handle)
{
	return s_AssetMap.at(handle);
}
