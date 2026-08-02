#pragma once

#include "Model.h"

class ModelImporter
{ 
public:
	static Model LoadModel(std::filesystem::path path);
};

