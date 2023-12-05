#pragma once

#include "PhysicsShape.h"

#include "Math/Transform.h"

class Rigidbody
{
public:
	struct Properties
	{
		float Mass = 1.0f;
		float Drag = 0.0f;
	};

	static Rigidbody* Create(Math::Transform& transform, Properties properties);
	static Rigidbody* Create(Math::Transform& transform, SphereShape& shape, Properties properties);
	static Rigidbody* Create(Math::Transform& transform, BoxShape& shape, Properties properties);

	virtual void SetMass(float mass) = 0;
	virtual void SetShape(BoxShape& shape) = 0;
	virtual void SetShape(SphereShape& shape) = 0;
	virtual Properties& GetProperties() = 0;
	virtual Math::Transform GetTransform() = 0;
	virtual void SetTransform(Math::Transform transform) = 0;
};

