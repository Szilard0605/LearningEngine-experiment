#pragma once

#include "Scene.h"

#include <string>

#include <filesystem>

class SceneSerializer
{
public:
	static void Serialize(Scene* scene, std::string filename);
	static Scene* Load(const std::filesystem::path path);
};