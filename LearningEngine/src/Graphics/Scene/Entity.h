#pragma once

#include "entt.hpp"
#include <string>

#include "Scene.h"

#include "Components.h"

class Entity
{
	public:
		Entity() = default;
		Entity(const std::string& name, entt::entity handle, Scene* scene);
		~Entity();

		std::string GetName() 
		{ 
			TagComponent& tag  = GetComponent<TagComponent>();
			return tag.Tag;
		}
		

		void SetHandle(entt::entity handle)
		{
			m_EntityHandle = handle;
		}

		entt::entity GetHandle()
		{
			return m_EntityHandle;
		}
		
		Scene* GetScene()
		{
			return m_Scene;
		}

		inline void SetParent(entt::entity parent)
		{
			// Set the parent
			HierarchyComponent& childHC = m_Scene->Registry.get<HierarchyComponent>(m_EntityHandle);
			childHC.Parent = (int)parent;

			// Add child to parent entity
			HierarchyComponent& parentHC  = m_Scene->Registry.get<HierarchyComponent>(parent);
			parentHC.Children.push_back((int)m_EntityHandle);
		}

		entt::entity GetParent()
		{
			HierarchyComponent& hc = m_Scene->Registry.get<HierarchyComponent>(m_EntityHandle);
			return (entt::entity)hc.Parent;
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
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			//HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->Registry.get<T>(m_EntityHandle);
		}

		bool operator ==(const Entity& other) const 
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

	private:
		Scene* m_Scene;
		std::string m_Name;
		entt::entity m_EntityHandle { entt::null };
		entt::entity ParentEntity = entt::null;
};
