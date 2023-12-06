#include "BulletRigidbody.h"

#include "btBulletDynamicsCommon.h"


BulletRigidbody::BulletRigidbody(Math::Transform& transform, Rigidbody::Properties properties)
{
	btCollisionShape* colShape = new btBoxShape({ transform.Scale.x, transform.Scale.y, transform.Scale.z });
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(transform.Rotation.y, transform.Rotation.x, transform.Rotation.z), btVector3(transform.Position.x, transform.Position.y, transform.Position.z)));
	
	btVector3 Inertia(0, 0, 0);
	colShape->calculateLocalInertia(properties.Mass, Inertia);
	
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(properties.Mass, motionState, colShape, Inertia);

	m_btRigidbody = new btRigidBody(groundRigidBodyCI);
}

BulletRigidbody::BulletRigidbody(Math::Transform& transform, SphereShape& shape, Rigidbody::Properties properties)
	: m_Transform(transform), m_Shape(shape)
{
	btCollisionShape* colShape = new btSphereShape(shape.Radius);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(transform.Rotation.y, transform.Rotation.x, transform.Rotation.z), btVector3(transform.Position.x, transform.Position.y, transform.Position.z)));

	btVector3 Inertia(0, 0, 0);
	colShape->calculateLocalInertia(properties.Mass, Inertia);

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(properties.Mass, motionState, colShape, Inertia);

	m_btRigidbody = new btRigidBody(groundRigidBodyCI);
}

BulletRigidbody::BulletRigidbody(Math::Transform& transform, BoxShape& shape, Rigidbody::Properties properties)
	: m_Transform(transform), m_Shape(shape)
{
	btCollisionShape* colShape = new btBoxShape({ shape.Size.x, shape.Size.y, shape.Size.z });
	
	btTransform defTransform;
	defTransform.setOrigin({ transform.Position.x, transform.Position.y, transform.Position.z });
	
	btDefaultMotionState* motionState = new btDefaultMotionState(defTransform);
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(properties.Mass, motionState, colShape, btVector3(0, 0, 0));
	
	m_btRigidbody = new btRigidBody(groundRigidBodyCI);
}

BulletRigidbody::~BulletRigidbody()
{
}

void BulletRigidbody::SetMass(float mass)
{
	m_Properties.Mass = mass;
	m_btRigidbody->setMassProps(mass, m_btRigidbody->getLocalInertia());
}

void BulletRigidbody::SetShape(BoxShape& shape)
{
	m_Shape = shape;
	m_btRigidbody->setCollisionShape(new btBoxShape({ shape.Size.x, shape.Size.y, shape.Size.z }));
}

void BulletRigidbody::SetShape(SphereShape& shape)
{
	m_Shape = shape;
	m_btRigidbody->setCollisionShape(new btSphereShape(shape.Radius));
}

Math::Transform BulletRigidbody::GetTransform()
{
	btTransform transform = m_btRigidbody->getWorldTransform();
	float yaw, pitch, roll;

	Math::Transform retTransform;
	retTransform.Position = { transform.getOrigin().x(),  transform.getOrigin().y(), transform.getOrigin().z()};
	
	transform.getBasis().getEulerZYX(yaw, pitch, roll);
	retTransform.Rotation = { roll, pitch, yaw };
	retTransform.Scale = m_Transform.Scale;

	return retTransform;
}

void BulletRigidbody::SetTransform(Math::Transform transform)
{
	btTransform retTransform;
	retTransform.setOrigin({transform.Position.x, transform.Position.y, transform.Position.z });
	retTransform.setRotation(btQuaternion(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z));
}
