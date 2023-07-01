#include "ShaderLibrary.h"

#include <map>
#include "Log/Log.h"

static std::map<std::string, Shader*> library;

void ShaderLibrary::Add(Shader* shader, const std::string name)
{
	library.emplace(name, shader);
}

Shader* ShaderLibrary::Load(const std::string& name, const std::string& path)
{
	Shader* shader = Shader::Create(path);
	Add(shader, name);
	return shader;
}

Shader* ShaderLibrary::GetShader(const std::string name)
{
	if (library[name])
	{
		return library[name];
	}

	LE_CORE_ERROR(std::string("Couldn't find ShaderLibrary named \"") + name + "\"");
	return nullptr;
}
