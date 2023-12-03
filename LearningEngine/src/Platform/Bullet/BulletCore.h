#pragma once

#include "Physics/PhysicsCore.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;

class BulletCore : public PhysicsCore
{
public:
	BulletCore();
	~BulletCore();

	btDefaultCollisionConfiguration* GetCollisionConfiguration() { return m_CollisionConfiguration; }
	btCollisionDispatcher* GetCollisionDispatcher() { return m_Dispatcher; }
	btBroadphaseInterface* GetBroadphaseInterface() { return m_BroadphaseInterface; }
	btSequentialImpulseConstraintSolver* SequentialImpulseConstraintSolver() { return m_Solver; };

private:
	btDefaultCollisionConfiguration* m_CollisionConfiguration;
	btCollisionDispatcher* m_Dispatcher;
	btBroadphaseInterface* m_BroadphaseInterface;
	btSequentialImpulseConstraintSolver* m_Solver;

};

