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

private:
	btDefaultCollisionConfiguration* collisionConfiguration ;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache ;
	btSequentialImpulseConstraintSolver* solver;

};

