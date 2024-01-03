#pragma once


#include <filesystem>

namespace sol
{
	class state;
}

class LuaScripting
{
public:
	static void Init();
	static void Shutdown();

	static void ExecuteScript(std::filesystem::path filePath);
	static void LoadScript(std::filesystem::path filePath);

	

	static sol::state* GetState();
};

