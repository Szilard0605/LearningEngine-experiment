#pragma once

#include "Asset.h"

class AssetLoader
{
public: 
	template<typename T>
	static T LoadAsset(AssetHandle handle);
};

