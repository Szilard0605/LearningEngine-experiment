#include "Entity.h"

Entity::Entity(const std::string& name, entt::entity handle, Scene* scene)
	: m_Name(name), m_EntityHandle(handle), m_Scene(scene)
{
	
}

Entity::~Entity()
{
	
}
