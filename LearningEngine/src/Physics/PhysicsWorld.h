#pragma once

#include "glm.hpp"

#include "Rigidbody.h"
#include "Graphics/Scene/Scene.h"

class Entity;

class PhysicsWorld 
{
public:
	static PhysicsWorld* Create(Scene* scene, glm::vec3 gravity);

	virtual void AddRigidBody(Rigidbody* body) = 0;
	virtual void StepSimulation(float timeStep) = 0;
	virtual void DestroyAllRigidbodies() = 0;
};

