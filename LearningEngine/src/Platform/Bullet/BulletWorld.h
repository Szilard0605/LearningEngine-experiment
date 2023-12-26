#pragma once

#include "Physics/PhysicsWorld.h"

#include "Graphics/Scene/Scene.h"

#include "BulletRigidbody.h"

class btDiscreteDynamicsWorld;

class BulletWorld : public PhysicsWorld
{
public:
	BulletWorld(Scene* scene, glm::vec3 gravity);
	~BulletWorld();

	virtual void AddRigidBody(Rigidbody* body) override;
	virtual void StepSimulation(float timeStep) override;
	virtual void DestroyAllRigidbodies() override;


private:
	Scene* m_Scene;
	std::vector<BulletRigidbody> m_Rigidbodies;
	btDiscreteDynamicsWorld* m_btWorld;
};

