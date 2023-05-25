#pragma once


#include "entt.hpp"
#include <string>

#include "Graphics/Camera/PerspectiveCamera.h"

class Entity;

class Scene
{
	public:
		Scene() = default;
		Scene(const std::string name);
		~Scene();

		Entity NewEntity(const std::string name);

		Entity& GetEntityByTag(std::string name);

		void Render(PerspectiveCamera& camera);

		const std::string& GetName() { return m_Name; }

		entt::registry Registry;
	private:
		std::string m_Name;
		friend class Entity;
};

