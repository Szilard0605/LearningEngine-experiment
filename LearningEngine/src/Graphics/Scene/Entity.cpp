#include "Entity.h"

Entity::Entity(const std::string& name, entt::entity handle, Scene* scene)
	: m_Name(name), m_EntityHandle(handle), m_Scene(scene)
{
	
}

Entity::~Entity()
{
}

template<typename T>
void Entity::AddComponent(T component)
{
	m_Scene->m_Registry.emplace(m_EntityHandle, component);
}