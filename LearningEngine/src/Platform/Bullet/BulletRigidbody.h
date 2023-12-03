#pragma once

#include "Physics/Rigidbody.h"

class btRigidBody;

class BulletRigidbody : public Rigidbody
{
public:
	BulletRigidbody(Math::Transform& transform, SphereShape& shape);
	BulletRigidbody(Math::Transform& transform, BoxShape& shape);
	~BulletRigidbody();

	virtual Math::Transform GetTransform() override;

	btRigidBody* GetBulletRigidbody() { return m_btRigidbody; }
	RigidbodyShape GetShape() { return m_Shape; }
private:
	RigidbodyShape m_Shape;

	Math::Transform m_Transform;
	// bullet
	btRigidBody* m_btRigidbody = nullptr;
};

