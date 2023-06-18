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
	m_Scene = new Scene(scene->GetName());
	m_Scene->Registry.assign(scene->Registry.data(), scene->Registry.data() + scene->Registry.size());
}

void EditorRuntime::Stop()
{
}

void EditorRuntime::Update(Timestep timeStep)
{

}