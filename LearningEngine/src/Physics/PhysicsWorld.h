#pragma once

#include "glm.hpp"

#include "Rigidbody.h"
#include "Graphics/Scene/Scene.h"

#include "Events/PhysicsEvents.h"
#include <functional>

class Entity;

class PhysicsWorld 
{
public:
	static PhysicsWorld* Create(Scene* scene, glm::vec3 gravity);

	virtual void AddRigidBody(Rigidbody* body) = 0;
	virtual void StepSimulation(float timeStep) = 0;
	virtual void DestroyAllRigidbodies() = 0;
	virtual Rigidbody* GetEntityRigidbody(entt::entity entityHandle) = 0;
	virtual void SetContactCallback(std::function<void(PhysicsContactEvent&)> callback) = 0;
};

