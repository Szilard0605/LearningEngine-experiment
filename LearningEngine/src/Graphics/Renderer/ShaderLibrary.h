#pragma once

#include "Core/Base.h"

#include "API/API.h"

class ShaderLibrary
{
public:
	static void Add(Shader* shader, const std::string name);
	static Shader* Load(const std::string& name, const std::string& path);
	static Shader* GetShader(const std::string name);
};