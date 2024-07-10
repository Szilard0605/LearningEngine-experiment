#include "LuaScriptInstance.h"
#include "LuaRegistry.h"

#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

#include "lstate.h"
#include "setjmp.h"

#include "Log/Log.h"

#include "Graphics/Scene/Entity.h"
#include "Physics/PhysicsWorld.h"

static jmp_buf s_LuaPanicJump;

static int Lua_AtPanicHandler(lua_State* lua)
{
	longjmp(s_LuaPanicJump, 1);
	return 0;
}

LuaScriptInstance::LuaScriptInstance(entt::entity entity, Scene* scene, std::filesystem::path path)
	: m_EntityHandle(entity), m_Scene(scene)
{
	m_State = new sol::state();
	m_State->open_libraries(sol::lib::base, sol::lib::math);

	lua_State* L = m_State->lua_state();
	L->l_G->panic = [](lua_State* L)
		{
			const char* errorMessage = lua_tostring(L, -1);
			LE_CORE_ERROR("Lua panic: %s", errorMessage);
			return 0;
		};

	lua_atpanic(L, &Lua_AtPanicHandler);
}

void LuaScriptInstance::LoadScript(std::filesystem::path path)
{

	sol::load_result loadResult = m_State->load_file(path.string(), sol::load_mode::text);
	if (!loadResult.valid())
	{
		sol::error error = loadResult;
		LE_CORE_ERROR("[LuaScriptInstance] Lua error: %s", error.what());
	}
	else
	{
		sol::protected_function_result functionResult = loadResult();
		if (!functionResult.valid())
		{
			sol::error error = functionResult;
			LE_CORE_ERROR("[LuaScriptInstance] Lua error: %s", error.what());
		}
	}

}

void LuaScriptInstance::OnCreate()
{
	auto& lua = *m_State;
	LUA_CALL("onCreate");
}

void LuaScriptInstance::OnUpdate(Timestep timestep)
{
	auto& lua = *m_State;
	LUA_CALL("onUpdate", timestep.Milliseconds());
}

void LuaScriptInstance::RegisterEntity()
{
	auto& lua = *m_State;

	// Input
	LUA_REGISTER_FUNCTION("isKeyPressed", [this](uint32_t keyCode) -> bool { return Input::IsKeyPressed(keyCode); });

	// Entity
	LUA_REGISTER_FUNCTION("getEntityByID", [this](uint32_t id) -> Entity { return GetEntityByID(id); });
	LUA_REGISTER_FUNCTION("getEntityByName", [this](const char* name) -> Entity { return GetEntityByName(name); });
	LUA_REGISTER_FUNCTION("getPosition", [this]() -> glm::vec3 { return Entity(m_EntityHandle, m_Scene).GetPosition(); });
	LUA_REGISTER_FUNCTION("setPosition", [this](glm::vec3 pos) { Entity(m_EntityHandle, m_Scene).SetPosition(pos); });

	// Phyiscs
	// TODO: Add rigidbody component to scripting so we can replace this.
	LUA_REGISTER_FUNCTION("rbApplyForce", [this](glm::vec3 force, glm::vec3 origin) { Rigidbody_ApplyForce(force, origin); });
	LUA_REGISTER_FUNCTION("rbWakeUp", [this](bool forceWakeUp) { Rigidbody_WakeUp(forceWakeUp); });
	LUA_REGISTER_FUNCTION("rbSleep", [this]() { Rigidbody_Sleep(); });
}


void LuaScriptInstance::Rigidbody_ApplyForce(glm::vec3 force, glm::vec3 origin)
{
	Rigidbody* rb = m_Scene->GetPhyiscsWorld()->GetEntityRigidbody(m_EntityHandle);
	if (!rb)
	{
		const char* entityName = Entity(m_EntityHandle, m_Scene).GetName().c_str();
		LE_CORE_ERROR("[Rigidbody_ApplyForce]: Entity %s is not a rigidbody", entityName);
		return;
	}

	rb->ApplyForce(force, origin);
}

void LuaScriptInstance::Rigidbody_WakeUp(bool forceWakeUp)
{
	Rigidbody* rb = m_Scene->GetPhyiscsWorld()->GetEntityRigidbody(m_EntityHandle);
	if (!rb)
	{
		Entity entity(m_EntityHandle, m_Scene);
		const char* entityName = entity.GetName().c_str();
		LE_CORE_ERROR("[Rigidbody_ApplyForce]: Entity %s is not a rigidbody", entityName);
		return;
	}

	rb->WakeUp(forceWakeUp);
}

void LuaScriptInstance::Rigidbody_Sleep()
{
	Rigidbody* rb = m_Scene->GetPhyiscsWorld()->GetEntityRigidbody(m_EntityHandle);
	if (!rb)
	{
		Entity entity(m_EntityHandle, m_Scene);
		const char* entityName = entity.GetName().c_str();
		LE_CORE_ERROR("[Rigidbody_ApplyForce]: Entity %s is not a rigidbody", entityName);
		return;
	}
	rb->Sleep();
}

Entity LuaScriptInstance::GetEntityByID(uint32_t id)
{
	return Entity((entt::entity)id, m_Scene);
}

Entity LuaScriptInstance::GetEntityByName(const char* name)
{
	Entity entity = m_Scene->GetEntityByTag(name);
	if (entity.IsValid())
		return entity;

	LE_CORE_ERROR("Couldn't find entity with name: %s", name);
	return Entity();
}
