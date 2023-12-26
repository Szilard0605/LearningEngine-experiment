#pragma once

#include "Physics/Rigidbody.h"

struct RigidbodyData;
class btRigidBody;

class BulletRigidbody : public Rigidbody
{
public:
	BulletRigidbody(Entity entity);
	BulletRigidbody(Entity entity, SphereShape& shape);
	BulletRigidbody(Entity entity, BoxShape& shape);
	//BulletRigidbody(Math::Transform& transform, BoxShape& shape);
	~BulletRigidbody();

	virtual float GetMass() override;
	virtual void SetMass(float mass) override;

	virtual float GetLinearDamping() override;
	virtual void SetLinearDamping(float damping) override;
	virtual float GetAngularDamping() override;
	virtual void SetAngularDamping(float damping) override;

	virtual void SetShape(BoxShape& shape) override;
	virtual void SetShape(SphereShape& shape) override;
	virtual Math::Transform GetTransform() override;
	virtual void SetTransform(Math::Transform transform) override;

	btRigidBody* GetBulletRigidbody() { return m_btRigidbody; }
	RigidbodyShape GetShape() { return m_Shape; }

	RigidbodyData* GetData() { return m_Data; };
private:

	RigidbodyShape m_Shape;

	
	RigidbodyData* m_Data;
	// bullet
	btRigidBody* m_btRigidbody = nullptr;
};

