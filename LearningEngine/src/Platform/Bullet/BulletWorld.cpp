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
	
	delete m_World;
}

void BulletWorld::AddRigidBody(Rigidbody* body)
{
	BulletRigidbody* bulletBody = (BulletRigidbody*)body;
	m_World->addRigidBody(bulletBody->GetBulletRigidbody());
}

void BulletWorld::Update(float timeStep)
{
	m_World->stepSimulation(timeStep, 1);

	int staticRbs = m_World->getNonStaticRigidBodies().size();
	int colObjs = m_World->getNumCollisionObjects();
	printf("static rigidbodies: %d\ncollision objects:%d\n", staticRbs, colObjs);
}

void BulletWorld::DestroyAllRigidbodies()
{
	for (int i = m_World->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		m_World->removeCollisionObject(m_World->getCollisionObjectArray()[i]);
	}
	int staticRbs = m_World->getNonStaticRigidBodies().size();
	int colObjs = m_World->getNumCollisionObjects();
	printf("---- DeletedAllBodies ---\nstatic rigidbodies: %d\ncollision objects: %d\n", staticRbs, colObjs);
}
