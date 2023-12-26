#include "BulletCore.h"

#include "btBulletDynamicsCommon.h"

BulletCore::BulletCore()
{
	m_CollisionConfiguration = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);


	m_BroadphaseInterface = new btDbvtBroadphase();
	m_Solver = new btSequentialImpulseConstraintSolver();
}

BulletCore::~BulletCore()
{
}
