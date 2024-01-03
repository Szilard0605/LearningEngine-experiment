#pragma once

#include "Core/Timestep.h"

#include "Graphics/Scene/Scene.h"

class Entity;

class LuaInterface
{
public:
	static void RegisterFunctions();
	static void RegisterEntity(Entity entity);
	static void OnCreateEntity(Entity entity);
	static void OnUpdateEntity(Entity entity, Timestep timestep);
	static void OnDestroyEntity(Entity entity);
	static void InitScene(Scene* scene);


};

