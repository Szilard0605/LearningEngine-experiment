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
	

	sol::state* m_State;
	Scene* m_Scene;
	entt::entity m_EntityHandle;
};

