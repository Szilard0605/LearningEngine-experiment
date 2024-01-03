#include "LuaInterface.h"
#include "LuaScripting.h"

#include "Graphics/Scene/Entity.h"
#include "Graphics/Scene/Components.h"

#include "sol.hpp"
#include <Log/Log.h>

#define LUA_CALL(Namespace, Function, ...) \
			lua[Namespace][Function](__VA_ARGS__)

#define LUA_REGISTER_FUNCTION(Namespace, Function, NativeFunction)\
			lua[Namespace][Function] = NativeFunction


void LuaInterface::RegisterFunctions()
{
	auto& lua = *LuaScripting::GetState();

	// vec3
	{
		auto mult_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 * f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f * v1; }
		);

		auto add_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 + v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 + f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f + v1; }
		);

		auto sub_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 - v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 - f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f - v1; }
		);
		auto div_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 / v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 / f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f / v1; }
		);

		lua.new_usertype<glm::vec3>("vec3",
			sol::constructors<glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			sol::meta_function::addition, add_overloads,
			sol::meta_function::subtraction, sub_overloads,
			sol::meta_function::multiplication, mult_overloads,
			sol::meta_function::division, div_overloads
		);
	}
}


void LuaInterface::RegisterEntity(Entity entity)
{
	auto& lua = *LuaScripting::GetState();
	lua.create_named_table(entity.GetName().c_str());
	LE_CORE_INFO("registering entity named table: %s", entity.GetName().c_str());
	LUA_REGISTER_FUNCTION(entity.GetName(), "getPosition", [&]() -> glm::vec3 { return entity.GetTransform().Position; });
	LUA_REGISTER_FUNCTION(entity.GetName(), "setPosition", [&](const glm::vec3 position) { entity.SetPosition(position); });
}

void LuaInterface::OnCreateEntity(Entity entity)
{
	auto& lua = *LuaScripting::GetState();
	LUA_CALL(entity.GetName().c_str(), "OnCreate");
}

void LuaInterface::OnUpdateEntity(Entity entity, Timestep timestep)
{
	auto& lua = *LuaScripting::GetState();
	LUA_CALL(entity.GetName().c_str(), "OnUpdate", timestep.Milliseconds());
}

void LuaInterface::OnDestroyEntity(Entity entity)
{
	auto& lua = *LuaScripting::GetState();
	LUA_CALL(entity.GetName().c_str(), "OnDestroy");
}

void LuaInterface::InitScene(Scene* scene)
{
	auto view = scene->Registry.view<LuaScriptComponent>();
	for (auto entity : view)
	{
		auto& lsc = view.get<LuaScriptComponent>(entity);
		std::string sourcePath = lsc.sourcePath;
		if (sourcePath.length())
		{
			RegisterEntity(Entity(entity, scene));
			LuaScripting::LoadScript(sourcePath);
			OnCreateEntity(Entity(entity, scene));
		}
	}
}
