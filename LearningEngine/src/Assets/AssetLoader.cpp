#include "AssetLoader.h"
#include "AssetManager.h"

#include "Log/Log.h"
#include "Graphics/Renderer/API/Texture2D.h"
#include "Graphics/Renderer/Model.h"

template<>
Texture2D* AssetLoader::LoadAsset<Texture2D*>(AssetHandle handle)
{
	Asset asset = AssetManager::GetAssetByHandle(handle);
	return Texture2D::Create(asset.Data.Path.string());
}

template<>
Model* AssetLoader::LoadAsset<Model*>(AssetHandle handle)
{
	Asset asset = AssetManager::GetAssetByHandle(handle);
	return new Model(asset.Data.Path.string());
}