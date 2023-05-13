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
	entt::entity entity = Registry.create();

	TransformComponent tc;
	Registry.emplace<TransformComponent>(entity, tc);

	TagComponent tagComponent;
	tagComponent.Tag = name;
	Registry.emplace<TagComponent>(entity, tagComponent);


	return Entity(name, entity, this);
}

Entity& Scene::GetEntityByTag(std::string name)
{
	auto view = Registry.view<TagComponent>();
	for (auto entity : view)
	{
		const TagComponent& tc = view.get<TagComponent>(entity);
		if (tc.Tag == name)
			return Entity (name, entity, this);
	}
}