#pragma once

#include "glm.hpp"

#include "Rigidbody.h"

class PhysicsWorld 
{
public:
	PhysicsWorld* Create(glm::vec3 gravity);

	virtual void AddRigidBody(Math::Transform& transform, Rigidbody* body) = 0;
	virtual void Update(float timeStep) = 0;
};

