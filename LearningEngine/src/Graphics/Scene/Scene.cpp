#include "Scene.h"

#include "Entity.h"

#include "Components.h"

#include "Graphics/Renderer/Renderer2D.h"


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

void Scene::Render(PerspectiveCamera& camera)
{
	Renderer2D::Begin(camera);

	Registry.each([this](auto entity)
	{
		if(Registry.has<QuadRendererComponent>(entity))
		{
			const TransformComponent& tc = Registry.get<TransformComponent>(entity);
			const QuadRendererComponent& qrc = Registry.get<QuadRendererComponent>(entity);
			Renderer2D::DrawQuad(tc.Position, tc.Size + qrc.Scale, tc.Rotation, qrc.Color);
		}
	});


	Renderer2D::End();
}
