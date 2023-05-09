#pragma once


#include "entt.hpp"
#include <string>

class Entity;


class Scene
{
	public:
	
		Scene(const std::string name);
		~Scene();

		Entity NewEntity(const std::string name);
	private:
		entt::registry m_Registry;
		std::string m_Name;
		friend class Entity;
};

