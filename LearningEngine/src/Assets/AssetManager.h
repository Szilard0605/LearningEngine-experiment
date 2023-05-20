#pragma once

#include "Assets.h"
#include "Asset.h"

class AssetManager
{
public:

	template<typename T, typename... Args>
	static Asset CreateAsset(Args... args);
};

