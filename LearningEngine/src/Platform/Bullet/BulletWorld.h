#pragma once

#include "Physics/PhysicsWorld.h"

class btDiscreteDynamicsWorld;

class BulletWorld : public PhysicsWorld
{
public:
	BulletWorld(glm::vec3 gravity);
	~BulletWorld();

	virtual void AddRigidBody(Math::Transform& transform, Rigidbody* body) override;
	virtual void Update(float timeStep) override;

private:
	btDiscreteDynamicsWorld* m_World;
};

