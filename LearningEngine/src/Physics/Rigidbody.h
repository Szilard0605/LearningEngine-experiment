#pragma once

#include "PhysicsShape.h"

#include "Math/Transform.h"

class Entity;

class Rigidbody
{
public:

	static Rigidbody* Create(Entity entity, SphereShape& shape);
	static Rigidbody* Create(Entity entity, BoxShape& shape);
	//static Rigidbody* Create(Math::Transform& transform, SphereShape& shape);
	//static Rigidbody* Create(Math::Transform& transform, BoxShape& shape);

	virtual float GetMass() = 0;
	virtual void SetMass(float mass) = 0;

	virtual float GetLinearDamping() = 0;
	virtual void SetLinearDamping(float damping) = 0;
	virtual float GetAngularDamping() = 0;
	virtual void SetAngularDamping(float damping) = 0;

	virtual void SetShape(BoxShape& shape) = 0;
	virtual void SetShape(SphereShape& shape) = 0;
	virtual Math::Transform GetTransform() = 0;
	virtual void SetTransform(Math::Transform transform) = 0;

};

