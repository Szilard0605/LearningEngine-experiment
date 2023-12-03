#include "BulletRigidbody.h"

#include "btBulletDynamicsCommon.h"


BulletRigidbody::BulletRigidbody(Math::Transform& transform, SphereShape& shape)
	: m_Transform(transform), m_Shape(shape)
{
	
}

BulletRigidbody::BulletRigidbody(Math::Transform& transform, BoxShape& shape)
	: m_Transform(transform), m_Shape(shape)
{
	btCollisionShape* colShape = new btBoxShape({ transform.Scale.x, transform.Scale.y, transform.Scale.z });
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, motionState, colShape, btVector3(0, 0, 0));
	
	m_btRigidbody = new btRigidBody(groundRigidBodyCI);
}

BulletRigidbody::~BulletRigidbody()
{
}

Math::Transform BulletRigidbody::GetTransform()
{
	btTransform transform = m_btRigidbody->getWorldTransform();
	float yaw, pitch, roll;

	Math::Transform retTransform;
	retTransform.Position = { transform.getOrigin().x(),  transform.getOrigin().y(), transform.getOrigin().z()};
	
	transform.getBasis().getEulerYPR(yaw, pitch, roll);
	retTransform.Rotation = { yaw, pitch, roll };
	retTransform.Scale = m_Transform.Scale;

	return retTransform;
}
