#pragma once
#include "entt.hpp"

#define LUA_CALL(Function, ...) lua[Function](__VA_ARGS__)

#define LUA_CALL_NAMESPACE(Namespace, Function, ...) \
			lua[Namespace][Function](__VA_ARGS__)

#define LUA_REGISTER_FUNCTION(Function, NativeFunction) \
			lua[Function] = NativeFunction

#define LUA_REGISTER_FUNCTION_NAMESPACE(Namespace, Function, NativeFunction)\
			lua[Namespace][Function] = NativeFunction

namespace sol { class state; }

class LuaRegistry
{
public:
	static void RegisterFunctions(sol::state* state);

};

