#pragma once


#include "entt.hpp"
#include <string>

#include "Graphics/Camera/PerspectiveCamera.h"

class Entity;

//TODO: COPY SCENE FUNCTION

class Scene
{
	public:
		Scene() = default;
		Scene(const std::string name);
		~Scene();

		Entity NewEntity(const std::string name);
		Entity& GetEntityByTag(std::string name);
		void Render(PerspectiveCamera* camera = nullptr);

		void SetName(const std::string name) { m_Name = name; }
		const std::string& GetName() { return m_Name; }
		
		void SetMainCamera(PerspectiveCamera* camera) { m_MainCamera = camera; }
		PerspectiveCamera* GetMainCamera() { return m_MainCamera; }

		entt::registry Registry;

	private:
		std::string m_Name;
		friend class Entity;

		PerspectiveCamera* m_MainCamera = nullptr;
};

