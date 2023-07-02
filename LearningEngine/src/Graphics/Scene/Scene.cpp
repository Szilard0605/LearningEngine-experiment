#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Graphics/Renderer/Renderer2D.h"
#include "Log/Log.h"

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

void Scene::Render(PerspectiveCamera* camera)
{
	PerspectiveCamera* mainCamera = camera;

	if (!mainCamera)
	{	
		glm::mat4 cameraTransform;
		{
			auto view = Registry.view<TransformComponent, PerspectiveCameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, PerspectiveCameraComponent>(entity);

				camera.Camera->Translate(transform.Position);
				
				if (camera.MainCamera)
				{
					mainCamera = camera.Camera;
					break;
				}
			}
		}
	}
	
	if (!mainCamera)
	{
		LE_CORE_ERROR("Scene: there is no main camera");
		return;
	}

	// Rendering 2D elements
	{
		Renderer2D::Begin(*mainCamera);

		auto view = Registry.view<TransformComponent, QuadRendererComponent>();

		for (auto entity : view)
		{
			auto [tc, qrc] = view.get<TransformComponent, QuadRendererComponent>(entity);
			Renderer2D::DrawQuad(tc.Position, tc.Size + qrc.Scale, tc.Rotation, qrc.Color);
		}

		Renderer2D::End();
	}

	// Rendering 3D Meshes
	{
		auto view = Registry.view<TransformComponent, StaticModelComponent>();

		for (auto entity : view)
		{
			auto [tc, smc] = view.get<TransformComponent, StaticModelComponent>(entity);
			smc.StaticModel->Render(*mainCamera, tc.Position, tc.Size, tc.Rotation);
		}
	}
}
