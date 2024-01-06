#include "LuaScriptEngine.h"

#include "LuaScriptInstance.h"
#include "Graphics/Scene/Entity.h"
#include "Graphics/Scene/Components.h"
#include "LuaRegistry.h"

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
		std::string sourcePath = lsc.sourcePath;
		if (sourcePath.length())
		{
			s_ScriptData.m_ScriptInstances.push_back(LuaScriptInstance(entity, scene, sourcePath));
			LuaScriptInstance& instance = s_ScriptData.m_ScriptInstances.front();

			LuaRegistry::RegisterFunctions(instance.GetState());
			instance.RegisterEntity();
			instance.LoadScript(sourcePath);

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
