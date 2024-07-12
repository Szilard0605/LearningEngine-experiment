#pragma once

#include "Graphics/Scene/Scene.h"

class LuaScriptEngine
{
public:
	static void InitScene(Scene* scene);
	static void UpdateScene(Scene* scene, Timestep timestep);
	static void OnPhysicsContact(Entity entityA, Entity entityB);
};

