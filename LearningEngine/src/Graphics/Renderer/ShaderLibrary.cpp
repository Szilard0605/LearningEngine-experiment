#include "ShaderLibrary.h"

#include <map>
#include "Log/Log.h"

static std::map<std::string, Shader*> s_Library;

void ShaderLibrary::Add(Shader* shader, const std::string name)
{
	s_Library.emplace(name, shader);
}

Shader* ShaderLibrary::Load(const std::string& name, const std::string& path)
{
	Shader* shader = Shader::Create(path);
	Add(shader, name);
	return shader;
}

Shader* ShaderLibrary::GetShader(const std::string name)
{
	if (s_Library[name])
	{
		return s_Library[name];
	}

	LE_CORE_ERROR(std::string("Couldn't find ShaderLibrary named \"") + name + "\"");
	return nullptr;
}

std::map<std::string, Shader*> ShaderLibrary::GetAllShaders()
{
	return s_Library;
}
