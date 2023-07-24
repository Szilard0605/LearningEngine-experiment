#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Graphics/Renderer/Renderer2D.h"
#include "Log/Log.h"

Scene::Scene(const std::string name)
	: m_Name(name)
{
	
}

Scene::Scene(Scene& other)
{

}

Scene::~Scene()
{
}

Entity Scene::NewEntity(const std::string name)
{
	entt::entity entity = Registry.create();

	HierarchyComponent hc;
	Registry.emplace<HierarchyComponent>(entity, hc);

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


template<typename... Component>
static void CopyComponent(entt::registry& dst, entt::registry& src, entt::entity srcEntity, entt::entity dstEntity)
{
	([&]()
	{
		if (src.try_get<Component>(srcEntity))
		{
			auto& srcComponent = src.get<Component>(srcEntity);
			dst.emplace_or_replace<Component>(dstEntity, srcComponent);

			if(dst.try_get<Component>(dstEntity))
				printf("added component to entity: %d\n", dstEntity);

		}
	}(), ...);
}


template<typename... Component>
static void CopyComponent(Components<Component...>, entt::registry& dst, entt::registry& src, entt::entity srcEntity, entt::entity dstEntity)
{
	CopyComponent<Component...>(dst, src, srcEntity, dstEntity);
}

Scene* Scene::Copy(Scene* scene)
{
	Scene* retScene = new Scene(scene->GetName());

	auto& srcRegistry = scene->Registry;
	auto& dstRegistry = retScene->Registry;

	auto view = srcRegistry.view<TransformComponent>();
	for (auto entity : view)
	{
		Entity newEntity = retScene->NewEntity(srcRegistry.get<TagComponent>(entity).Tag);

		CopyComponent(EveryComponent{}, dstRegistry, srcRegistry, entity, newEntity.GetHandle());
	}

	return retScene;
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
			if (qrc.enabled)
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

void Scene::OnViewportResize(uint32_t width, uint32_t height)
{
	auto view = Registry.view<PerspectiveCameraComponent>();
	for (auto entity : view)
	{
		auto camera = view.get<PerspectiveCameraComponent>(entity);

		if (camera.FixedAspectRatio)
		{
			camera.AspectRatio = (float)width / (float)height;
			camera.Camera->SetAspectRatio(camera.AspectRatio);
		}
	}
}