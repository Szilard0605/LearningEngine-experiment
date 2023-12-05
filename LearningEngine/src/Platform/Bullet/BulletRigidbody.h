#pragma once

#include "Physics/Rigidbody.h"

class btRigidBody;

class BulletRigidbody : public Rigidbody
{
public:
	BulletRigidbody(Math::Transform& transform, Rigidbody::Properties properties);
	BulletRigidbody(Math::Transform& transform, SphereShape& shape, Rigidbody::Properties properties);
	BulletRigidbody(Math::Transform& transform, BoxShape& shape, Rigidbody::Properties properties);
	~BulletRigidbody();

	virtual void SetMass(float mass) override;
	virtual void SetShape(BoxShape& shape) override;
	virtual void SetShape(SphereShape& shape) override;
	virtual Rigidbody::Properties& GetProperties() override { return m_Properties; };
	virtual Math::Transform GetTransform() override;
	virtual void SetTransform(Math::Transform transform) override;

	btRigidBody* GetBulletRigidbody() { return m_btRigidbody; }
	RigidbodyShape GetShape() { return m_Shape; }
private:
	RigidbodyShape m_Shape;

	Math::Transform m_Transform;

	Rigidbody::Properties m_Properties;

	// bullet
	btRigidBody* m_btRigidbody = nullptr;
};

