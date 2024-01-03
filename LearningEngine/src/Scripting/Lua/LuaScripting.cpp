#include "LuaScripting.h"
#include "LuaInterface.h"

#include <Log/Log.h>

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

#include "lstate.h"
#include "setjmp.h"

struct LuaData
{
	sol::state* solState;
};

static LuaData s_LuaData;

static jmp_buf s_LuaPanicJump;

static int Lua_AtPanicHandler(lua_State* lua)
{
	longjmp(s_LuaPanicJump, 1);
	return 0;
}

void LuaScripting::Init()
{
	LE_CORE_INFO("[Lua Scripting] Initializing");

	s_LuaData.solState = new sol::state();
	s_LuaData.solState->open_libraries(sol::lib::base, sol::lib::math);

	lua_State* L = s_LuaData.solState->lua_state();
	L->l_G->panic = [](lua_State* L)
	{
		const char* errorMessage = lua_tostring(L, -1);
		LE_CORE_ERROR("Lua panic: %s", errorMessage);
		return 0;
	};

	lua_atpanic(L, &Lua_AtPanicHandler);

	LuaInterface::RegisterFunctions();

	LE_CORE_INFO("[Lua Scripting] Initialized");
}

void LuaScripting::Shutdown()
{
	delete s_LuaData.solState;
	s_LuaData.solState = nullptr;
}

void LuaScripting::ExecuteScript(std::filesystem::path filePath)
{
	auto result = s_LuaData.solState->script_file(filePath.string().c_str(), [filePath](lua_State*, sol::protected_function_result result)
	{
			LE_CORE_ERROR("error while trying to execute Lua script: %s", filePath.string().c_str());
			return result;
	});

}

void LuaScripting::LoadScript(std::filesystem::path filePath)
{
	sol::load_result loadResult = s_LuaData.solState->load_file(filePath.string(), sol::load_mode::text);
	if (!loadResult.valid())
	{
		sol::error error = loadResult;
		LE_CORE_ERROR("[ScriptEngine] Lua error: %s", error.what());
	}
	else
	{
		sol::protected_function_result functionResult = loadResult();
		if (!functionResult.valid())
		{
			sol::error error = functionResult;
			LE_CORE_ERROR("[ScriptEngine] Lua error: %s", error.what());
		}
	}

}

sol::state* LuaScripting::GetState()
{
	return s_LuaData.solState;
}
