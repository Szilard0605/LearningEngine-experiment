#include "EditorRuntime.h"

#include "entt.hpp"

EditorRuntime::EditorRuntime()
{
	m_PhysicsWorld = PhysicsWorld::Create({ 0, -9.8f, 0 });
}

EditorRuntime::~EditorRuntime()
{
}

void EditorRuntime::Start(Scene* scene)
{
	m_Scene = Scene::Copy(scene);
	{
		auto view = m_Scene->Registry.view<RigidbodyComponent, BoxColliderComponent>();
		for (auto entity : view)
		{
			auto [rc, bcc] = view.get<RigidbodyComponent, BoxColliderComponent>(entity);
			TransformComponent& tc = m_Scene->Registry.get<TransformComponent>(entity);
			rc.Rigidbody = Rigidbody::Create(tc.Transform, rc.Properties);
			rc.Rigidbody->SetShape(BoxShape(bcc.Size));
			m_PhysicsWorld->AddRigidBody(rc.Rigidbody);
		}
	}

	{
		auto view = m_Scene->Registry.view<RigidbodyComponent, SphereColliderComponent>();
		for (auto entity : view)
		{
			auto [rc, scc] = view.get<RigidbodyComponent, SphereColliderComponent>(entity);
			TransformComponent& tc = m_Scene->Registry.get<TransformComponent>(entity);
			rc.Rigidbody = Rigidbody::Create(tc.Transform, rc.Properties);
			rc.Rigidbody->SetShape(SphereShape(scc.Radius));
			m_PhysicsWorld->AddRigidBody(rc.Rigidbody);
		}
	}
}

void EditorRuntime::Stop()
{
	delete m_Scene;
	
	m_PhysicsWorld->DestroyAllRigidbodies();
}

void EditorRuntime::Update(Timestep timeStep)
{
	if (!m_Scene)
		return;

	m_Scene->Render();

	if (m_PhysicsWorld)
	{
		m_PhysicsWorld->Update((1.0f / 60.0f));

		auto view = m_Scene->Registry.view<RigidbodyComponent>();
		for (auto entity : view)
		{
			auto rc = view.get<RigidbodyComponent>(entity);
			TransformComponent& tc = m_Scene->Registry.get<TransformComponent>(entity);
			tc.Transform.Position = rc.Rigidbody->GetTransform().Position;
			tc.Transform.Rotation = rc.Rigidbody->GetTransform().Rotation;
		}
	}
}