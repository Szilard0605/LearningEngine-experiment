#pragma once

#include "entt.hpp"
#include "Graphics/Scene/Scene.h"

#include <filesystem>

namespace sol { class state; }

class LuaScriptInstance
{
public:
	LuaScriptInstance(entt::entity entity, Scene* scene, std::filesystem::path path);

	void LoadScript(std::filesystem::path path);
	void OnCreate();
	void OnUpdate(Timestep timestep);
	void RegisterEntity();

	sol::state* GetState() { return m_State; }

private:

	void Rigidbody_ApplyForce(glm::vec3 force, glm::vec3 origin);
	void Rigidbody_WakeUp(bool forceWakeUp);
	void Rigidbody_Sleep();
	
	Entity GetEntityByID(uint32_t id);
	Entity GetEntityByName(const char* name);

	sol::state* m_State;
	Scene* m_Scene;
	entt::entity m_EntityHandle;
};

