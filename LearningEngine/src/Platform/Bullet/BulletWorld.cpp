#include "BulletWorld.h"
#include "BulletCore.h"
#include "Core/Application.h"

#include "btBulletDynamicsCommon.h"
#include "BulletRigidbody.h"

BulletWorld::BulletWorld(glm::vec3 gravity)
{
	BulletCore* core = (BulletCore*)Application::GetInstance()->GetPhysicsCore();
	m_World = new btDiscreteDynamicsWorld(core->GetCollisionDispatcher(), core->GetBroadphaseInterface(), core->SequentialImpulseConstraintSolver(), core->GetCollisionConfiguration());
	m_World->setGravity({ gravity.x, gravity.y, gravity.z });
}

BulletWorld::~BulletWorld()
{
}

void BulletWorld::AddRigidBody(Math::Transform& transform, Rigidbody* body)
{
	BulletRigidbody* bulletBody = (BulletRigidbody*)body;
	m_World->addRigidBody(bulletBody->GetBulletRigidbody());
}

void BulletWorld::Update(float timeStep)
{
	m_World->stepSimulation(timeStep, 1);
}
