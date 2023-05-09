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

		void SetHandle(entt::entity handle)
		{
			m_EntityHandle = handle;
		}

		entt::entity GetHandle()
		{
			return m_EntityHandle;
		}

		template<typename T>
		void AddComponent(T component);

		template<typename T>
		bool HasComponent(T component);


	private:
		Scene* m_Scene;
		std::string m_Name;
		entt::entity m_EntityHandle { entt::null };
};
