#pragma once

#include "Physics/PhysicsWorld.h"

class btDiscreteDynamicsWorld;

class BulletWorld : public PhysicsWorld
{
public:
	BulletWorld(glm::vec3 gravity);
	~BulletWorld();

	virtual void AddRigidBody(Rigidbody* body) override;
	virtual void Update(float timeStep) override;
	virtual void DestroyAllRigidbodies() override;

private:
	btDiscreteDynamicsWorld* m_World;
};

