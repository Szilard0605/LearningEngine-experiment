#pragma once

#include "glm.hpp"

#include "Rigidbody.h"

class PhysicsWorld 
{
public:
	static PhysicsWorld* Create(glm::vec3 gravity);

	virtual void AddRigidBody(Rigidbody* body) = 0;
	virtual void Update(float timeStep) = 0;
	virtual void DestroyAllRigidbodies() = 0;
};

