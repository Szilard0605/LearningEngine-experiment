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

	TransformComponent tc;
	Registry.emplace<TransformComponent>(entity, tc);

	TagComponent tagComponent;
	tagComponent.Tag = name;
	Registry.emplace<TagComponent>(entity, tagComponent);

	HierarchyComponent hc;
	Registry.emplace<HierarchyComponent>(entity, hc);

	return Entity(name, entity, this);
}

void Scene::DestroyEntity(Entity& entity)
{
	HierarchyComponent& hc = entity.GetComponent<HierarchyComponent>();
	Entity parent(hc.Parent, this);
	if (parent.IsValid())
	{
		HierarchyComponent& parentHC = parent.GetComponent<HierarchyComponent>();
		for (auto it = parentHC.Children.begin(); it < parentHC.Children.end(); it++)
		{
			if (*it == entity.GetHandle())
			{
				parentHC.Children.erase(it);
			}
		}
	}

	hc.Parent = entt::null;

	if (hc.Children.size())
	{
		for (auto it = hc.Children.begin(); it < hc.Children.end(); it++)
		{
			hc.Children.erase(it);
		}
	}

	Registry.destroy(entity.GetHandle());
}

Entity& Scene::GetEntityByTag(std::string name)
{
	auto view = Registry.view<TagComponent>();
	for (auto entity : view)
	{
		const TagComponent& tc = view.get<TagComponent>(entity);
		if (tc.Tag == name)
			return Entity (entity, this);
	}

	return Entity();
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

	// Render Entity hierarchy
	auto view = scene->Registry.view<TransformComponent>();

	for (auto it = view.begin(); it < view.end(); it++)
	{
		Entity newEntity = retScene->NewEntity(srcRegistry.get<TagComponent>(view[it.index()]).Tag);

		CopyComponent(EveryComponent{}, dstRegistry, srcRegistry, view[it.index()], newEntity.GetHandle());
	}

	return retScene;
}


void Scene::Render(PerspectiveCamera* camera)
{
	PerspectiveCamera* mainCamera = camera;

	if (!mainCamera)
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
				Renderer2D::DrawQuad(tc.Position, tc.Size + qrc.Scale, glm::degrees(tc.Rotation), qrc.Color, (int)entity);
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

void Scene::OnViewportResize(float width, float height)
{
	auto view = Registry.view<PerspectiveCameraComponent>();
	for (auto entity : view)
	{
		auto camera = view.get<PerspectiveCameraComponent>(entity);

		if (camera.FixedAspectRatio)
		{
			camera.AspectRatio = width / height;
			camera.Camera->SetAspectRatio(camera.AspectRatio);
		}
	}
}