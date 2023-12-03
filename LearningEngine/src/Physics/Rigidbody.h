#pragma once

#include "PhysicsShape.h"

#include "Math/Transform.h"

class Rigidbody
{
public:
	static Rigidbody* Create(Math::Transform& transform, SphereShape& shape);
	static Rigidbody* Create(Math::Transform& transform, BoxShape& shape);

	virtual Math::Transform GetTransform() = 0;
};

