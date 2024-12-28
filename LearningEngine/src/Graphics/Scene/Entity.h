#pragma once

#include "entt.hpp"
#include <string>

#include "Scene.h"

#include "Log/Log.h"

#include "Components.h"

class Entity
{
	public:
		Entity() = default;

		Entity(entt::entity handle, Scene* scene) 
			: m_EntityHandle(handle), m_Scene(scene) { }

		~Entity()
		{

		}

		std::string GetName() 
		{ 
			TagComponent& tag  = GetComponent<TagComponent>();
			return tag.Tag;
		}
		
		std::string GetNameCStr()
		{
			TagComponent& tag = GetComponent<TagComponent>();
			return tag.Tag.c_str();
		}

		bool IsValid()
		{
			return m_EntityHandle != entt::null;
		}

		void SetHandle(entt::entity handle)
		{
			m_EntityHandle = handle;
		}

		entt::entity GetHandle()
		{
			return m_EntityHandle;
		}
		
		void SetScene(Scene* scene)
		{
			m_Scene = scene;
		}

		Scene* GetScene()
		{
			return m_Scene;
		}

		inline void SetParent(Entity entity)
		{
			// Set the parent
			HierarchyComponent& childHC = GetComponent<HierarchyComponent>();
			childHC.Parent = entity.GetHandle();

			// Add child to parent entity
			HierarchyComponent& parentHC  = entity.GetComponent<HierarchyComponent>();
			parentHC.Children.push_back(m_EntityHandle);
		}

		Entity GetParent()
		{
			HierarchyComponent& hc = GetComponent<HierarchyComponent>();
			return Entity(hc.Parent, m_Scene);
		}

		std::vector<Entity> GetChildren()
		{
			HierarchyComponent& hc = GetComponent<HierarchyComponent>();

			std::vector<Entity> children;
			for (int i = 0; i < hc.Children.size(); i++)
			{
				children.push_back(Entity(hc.Children[i], m_Scene));
			}

			return children;
		}

		void AddChildren(Entity entity)
		{
			HierarchyComponent& hc = GetComponent<HierarchyComponent>();
			hc.Children.push_back(entity.GetHandle());
		}

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			m_Scene->Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			//m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->Registry.try_get<T>(m_EntityHandle) != nullptr;
		}

		template<typename T>
		T& GetComponent()
		{
			//HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->Registry.get<T>(m_EntityHandle);
		}

		void SetPosition(glm::vec3 position)
		{
			TransformComponent& tc = GetComponent<TransformComponent>();
			tc.Transform.Position = position;
		}

		glm::vec3 GetPosition() 
		{
			TransformComponent& tc = GetComponent<TransformComponent>();
			return tc.Transform.Position;
		}

		glm::vec3 GetRotation()
		{
			TransformComponent& tc = GetComponent<TransformComponent>();
			return tc.Transform.Rotation;
		}

		void SetRotation(glm::vec3 rotation)
		{
			TransformComponent& tc = GetComponent<TransformComponent>();
			tc.Transform.Rotation = rotation;
		}

		void SetTransform(Math::Transform transform)
		{
			HierarchyComponent& hc = GetComponent<HierarchyComponent>();
			for (auto it_child : hc.Children)
			{
				Entity child(it_child, m_Scene);
				if (child.IsValid())
				{
					Math::Transform deltaTransform = GetTransform();
					deltaTransform.Position = transform.Position - deltaTransform.Position;
					deltaTransform.Rotation = transform.Rotation - deltaTransform.Rotation;

					Math::Transform transform = child.GetTransform();
					transform.Position += deltaTransform.Position;
					transform.Rotation += deltaTransform.Rotation;

					child.SetTransform(transform);
				}
			}

			TransformComponent& tc = GetComponent<TransformComponent>();
			tc.Transform = transform;

		}

		Math::Transform GetTransform()
		{
			return GetComponent<TransformComponent>().Transform;
		}

		bool operator ==(entt::entity other) const 
		{
			return m_EntityHandle == other;
		}

		bool operator ==(const Entity& other) const 
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator !=(const Entity& other) const
		{
			return m_EntityHandle != other.m_EntityHandle || m_Scene != other.m_Scene;
		}

	private:
		Scene* m_Scene;
		entt::entity m_EntityHandle { entt::null };
};
