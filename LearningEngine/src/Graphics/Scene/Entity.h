#pragma once

#include "entt.hpp"
#include <string>

#include "Scene.h"

class Entity
{
	public:
		Entity() = default;
		Entity(const std::string& name, entt::entity handle, Scene* scene);
		~Entity();

		std::string GetName() { return m_Name; }
		

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

		void SetParent(entt::entity parent)
		{
			ParentEntity = parent;
		}

		entt::entity GetParent()
		{
			return ParentEntity;
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
