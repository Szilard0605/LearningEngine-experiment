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
}

void EditorRuntime::Stop()
{
}

void EditorRuntime::Update(Timestep timeStep)
{
	m_Scene->Render();
}