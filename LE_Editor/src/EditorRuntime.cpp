#include "EditorRuntime.h"

#include "entt.hpp"

EditorRuntime::EditorRuntime()
{
	
}

EditorRuntime::~EditorRuntime()
{
}



void EditorRuntime::Start(Scene* scene)
{
	m_Scene = Scene::Copy(scene);
	m_Scene->OnStart();
}

void EditorRuntime::Stop()
{
	m_Scene->OnStop();
	delete m_Scene;
}

void EditorRuntime::Update(Timestep timeStep)
{
	if (!m_Scene)
		return;

	m_Scene->Render();
	m_Scene->StepPhysicsSimulation((1.0f / 30.0f));
}