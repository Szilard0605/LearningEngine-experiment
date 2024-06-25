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
	LUA_REGISTER_FUNCTION("getPosition", [this]() -> glm::vec3 { return Entity(m_EntityHandle, m_Scene).GetPosition(); });
	LUA_REGISTER_FUNCTION("setPosition", [this](glm::vec3 pos) { Entity(m_EntityHandle, m_Scene).SetPosition(pos); });

	// Phyiscs
	LUA_REGISTER_FUNCTION("rbApplyForce", [this](glm::vec3 force, glm::vec3 origin) { Rigidbody_ApplyForce(force, origin); });
	LUA_REGISTER_FUNCTION("rbWakeUp", [this](bool forceWakeUp) { Rigidbody_WakeUp(forceWakeUp); });
	LUA_REGISTER_FUNCTION("rbSleep", [this]() { Rigidbody_Sleep(); });
}


void LuaScriptInstance::Rigidbody_ApplyForce(glm::vec3 force, glm::vec3 origin)
{
	Rigidbody* rb = m_Scene->GetPhyiscsWorld()->GetEntityRigidbody(m_EntityHandle);
	if (!rb)
	{
		LE_CORE_ERROR("[Rigidbody_ApplyForce]: There is no rigidbody");
		return;
	}

	rb->ApplyForce(force, origin);
}

void LuaScriptInstance::Rigidbody_WakeUp(bool forceWakeUp)
{
	Rigidbody* rb = m_Scene->GetPhyiscsWorld()->GetEntityRigidbody(m_EntityHandle);
	if (!rb)
	{
		LE_CORE_ERROR("[Rigidbody_WakeUp]: There is no rigidbody");
		return;
	}

	rb->WakeUp(forceWakeUp);
}

void LuaScriptInstance::Rigidbody_Sleep()
{
	Rigidbody* rb = m_Scene->GetPhyiscsWorld()->GetEntityRigidbody(m_EntityHandle);
	if (!rb)
	{
		LE_CORE_ERROR("[Rigidbody_Sleep]: There is no rigidbody");
		return;
	}
	rb->Sleep();
}
