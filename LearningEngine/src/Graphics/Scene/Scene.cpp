#include "Scene.h"

#include "Entity.h"

#include "Components.h"

Scene::Scene(const std::string name)
	: m_Name(name)
{
	
}

Scene::~Scene()
{
}

Entity Scene::NewEntity(const std::string name)
{
	entt::entity entity = m_Registry.create();
	TransformComponent tc;
	m_Registry.emplace<TransformComponent>(entity, tc);

	return Entity(name, entity, this);
}