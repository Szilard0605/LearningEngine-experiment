#pragma once


#include "entt.hpp"
#include <string>

#include "Graphics/Camera/PerspectiveCamera.h"

#include "Graphics/Renderer/API/Shader.h"
#include "Graphics/Renderer/API/CubeMap.h"

#include "Graphics/Renderer/Light.h"
//#include "Physics/PhysicsWorld.h"

class Entity;
class PhysicsWorld;

class Scene
{
	public:
		Scene() = default;
		Scene(const std::string name);
		~Scene();

		Entity NewEntity(const std::string name);
		void DestroyEntity(Entity entity);
		Entity GetEntityByTag(std::string name);
		void OnStart();
		void Render(PerspectiveCamera* camera = nullptr);
		void StepPhysicsSimulation(float timestep);
		void OnStop();

		void SetName(const std::string name) { m_Name = name; }
		const std::string& GetName() { return m_Name; }
		
		void SetMainCamera(PerspectiveCamera* camera) { m_MainCamera = camera; }
		PerspectiveCamera* GetMainCamera() { return m_MainCamera; }

		void OnViewportResize(float width, float height);

		static Scene* Copy(Scene* scene);

		inline void SetAmbientLight(AmbientLight light) { m_AmbientLight = light; }
		inline AmbientLight GetAmbientLight() { return m_AmbientLight; }

		entt::registry Registry;

	private:
		std::string m_Name;
		friend class Entity;

		AmbientLight m_AmbientLight = { glm::vec3(1.0f),  1};

		PerspectiveCamera* m_MainCamera = nullptr;

		PhysicsWorld* m_PhysicsWorld = nullptr;
};

