#include "AssetRegistry.h"

#include <string>
#include <fstream>
#include "json.hpp"

#include "AssetManager.h"

#include <functional>

using namespace nlohmann;

// Note (Szilard): This function is not tested yet
void AssetRegistry::LoadAssets(std::filesystem::path path)
{
    json jsonObj;

    std::ifstream AssetFile(path.c_str());
    if (AssetFile.is_open())
    {
        jsonObj = json::parse(AssetFile);
        AssetFile.close();
        return;
    }
    //Asset asset = s_DeserializerFunctions.at(jsonObj["Type"])(jsonObj);

    for (json::iterator it = jsonObj.begin(); it != jsonObj.end(); ++it)
    {  
        AssetData data;

        std::string handle = it.key();
        data.Type = jsonObj[handle]["Type"];
        std::string pathStr = jsonObj[handle]["FilePath"];
        data.Path = std::filesystem::path(pathStr);

        AssetManager::CreateAsset(data);
    }
}

void AssetRegistry::SaveAsset(AssetHandle handle, std::filesystem::path path)
{
    json jsonObj;

    std::ifstream inAssetFile(path);

    if (inAssetFile.is_open())
    {
        jsonObj = json::parse(inAssetFile);
        inAssetFile.close();
    }

    Asset& asset = AssetManager::GetAssetByHandle(handle);

    std::string handleStr = std::to_string(handle);
    jsonObj[handleStr] = json::object();
    jsonObj[handleStr]["Type"] = asset.Data.Type;
    jsonObj[handleStr]["FilePath"] = asset.Data.Path.string();

    std::ofstream outAssetFile(path);
    outAssetFile << std::setw(4) << jsonObj << '\n';
    outAssetFile.close();
}
