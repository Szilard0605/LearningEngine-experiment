#include "LuaScriptEngine.h"

#include "LuaScriptInstance.h"
#include "Graphics/Scene/Entity.h"
#include "Graphics/Scene/Components.h"
#include "LuaRegistry.h"
#include <Log/Log.h>

struct ScriptData
{
	std::vector<LuaScriptInstance> m_ScriptInstances;
};

static ScriptData s_ScriptData;

void LuaScriptEngine::InitScene(Scene* scene)
{
	s_ScriptData.m_ScriptInstances.clear();
	auto view = scene->Registry.view<LuaScriptComponent>();
	for (auto entity : view)
	{
		auto& lsc = view.get<LuaScriptComponent>(entity);
		if (lsc.SourcePath.length())
		{
			s_ScriptData.m_ScriptInstances.push_back(LuaScriptInstance(entity, scene, lsc.SourcePath));
			LuaScriptInstance& instance = s_ScriptData.m_ScriptInstances.front();

			LuaRegistry::RegisterFunctions(instance.GetState());
			instance.RegisterEntity();
			instance.LoadScript(lsc.SourcePath);

			instance.OnCreate();
		}
	}
}

void LuaScriptEngine::UpdateScene(Scene* scene, Timestep timestep)
{
	for (int i = 0; i < s_ScriptData.m_ScriptInstances.size(); i++)
	{
		s_ScriptData.m_ScriptInstances[i].OnUpdate(timestep);
	}
}

void LuaScriptEngine::OnPhysicsContact(Entity entityA, Entity entityB)
{
	for (int i = 0; i < s_ScriptData.m_ScriptInstances.size(); i++)
	{
		auto& instance = s_ScriptData.m_ScriptInstances[i];
		//LE_CORE_INFO("instance[%d]: %d, entityA: %d, entityB: %d", i, instance.GetEntityHandle(), entityA.GetHandle(), entityB.GetHandle());
		if (entityA.GetHandle() == instance.GetEntityHandle())
		{
			instance.OnPhysicsContact(entityB);
		}

		if (entityB.GetHandle() == instance.GetEntityHandle())
		{
			instance.OnPhysicsContact(entityA);
		}
	}
}
