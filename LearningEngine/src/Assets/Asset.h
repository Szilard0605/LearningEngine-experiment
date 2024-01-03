#pragma once

#include <string>
#include <filesystem>

enum class AssetType : uint16_t
{
	None, Texture2D, Model, Scene, LuaScript
};

typedef uint32_t AssetHandle;

struct AssetData
{
	AssetType Type;
	std::filesystem::path Path;
};

class Asset
{
public:

	Asset() = default;

	Asset(AssetData data)
		: Data(data)
	{

	}
	~Asset() = default;

	AssetData Data;
};

