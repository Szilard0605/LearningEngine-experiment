#include "BulletRigidbody.h"

#include "btBulletDynamicsCommon.h"

#include "Graphics/Scene/Entity.h"

struct RigidbodyData
{
	Entity EntityID;
};

BulletRigidbody::BulletRigidbody(Entity entity)
{
	m_Data = new RigidbodyData();
	m_Data->EntityID = entity;

	Math::Transform transform = entity.GetComponent<TransformComponent>().Transform;

	btCollisionShape* colShape = new btSphereShape(0.0f);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(transform.Rotation.y, transform.Rotation.x, transform.Rotation.z), btVector3(transform.Position.x, transform.Position.y, transform.Position.z)));

	btVector3 Inertia(0, 0, 0);
	colShape->calculateLocalInertia(1.0f, Inertia);

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(1.0f, motionState, colShape, Inertia);

	m_btRigidbody = new btRigidBody(groundRigidBodyCI);
	m_btRigidbody->setUserPointer(this);
}

BulletRigidbody::BulletRigidbody(Entity entity, BoxShape& shape)
{
	m_Data = new RigidbodyData();
	m_Data->EntityID = entity;

	Math::Transform transform = entity.GetComponent<TransformComponent>().Transform;

	btCollisionShape* colShape = new btBoxShape({ shape.Size.x, shape.Size.y, shape.Size.z });
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(transform.Rotation.y, transform.Rotation.x, transform.Rotation.z), btVector3(transform.Position.x, transform.Position.y, transform.Position.z)));
	
	btVector3 Inertia(0, 0, 0);
	colShape->calculateLocalInertia(1.0f, Inertia);
	
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(1.0f, motionState, colShape, Inertia);

	m_btRigidbody = new btRigidBody(groundRigidBodyCI);
	m_btRigidbody->setUserPointer(this);
}

BulletRigidbody::BulletRigidbody(Entity entity, SphereShape& shape)
{
	m_Data = new RigidbodyData();
	m_Data->EntityID = entity;

	Math::Transform transform = entity.GetComponent<TransformComponent>().Transform;

	btCollisionShape* colShape = new btSphereShape(shape.Radius);
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(transform.Rotation.y, transform.Rotation.x, transform.Rotation.z), btVector3(transform.Position.x, transform.Position.y, transform.Position.z)));

	btVector3 Inertia(0, 0, 0);
	colShape->calculateLocalInertia(1.0f, Inertia);

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(1.0f, motionState, colShape, Inertia);

	m_btRigidbody = new btRigidBody(groundRigidBodyCI);
	m_btRigidbody->setUserPointer(this);
}

/*BulletRigidbody::BulletRigidbody(Math::Transform& transform, BoxShape& shape)
	: m_Transform(transform), m_Shape(shape)
{
	btCollisionShape* colShape = new btBoxShape({ shape.Size.x, shape.Size.y, shape.Size.z });
	
	btTransform defTransform;
	defTransform.setOrigin({ transform.Position.x, transform.Position.y, transform.Position.z });
	
	btDefaultMotionState* motionState = new btDefaultMotionState(defTransform);
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(1.0f, motionState, colShape, btVector3(0, 0, 0));
	
	m_btRigidbody = new btRigidBody(groundRigidBodyCI);
}*/

BulletRigidbody::~BulletRigidbody()
{
}

float BulletRigidbody::GetMass()
{
	return m_btRigidbody->getMass();
}

void BulletRigidbody::SetMass(float mass)
{
	m_btRigidbody->setMassProps(mass, m_btRigidbody->getLocalInertia());
}

float BulletRigidbody::GetLinearDamping()
{
	return m_btRigidbody->getLinearDamping();
}

void BulletRigidbody::SetLinearDamping(float damping)
{
	m_btRigidbody->setDamping(damping, m_btRigidbody->getAngularDamping());
}

float BulletRigidbody::GetAngularDamping()
{
	return m_btRigidbody->getAngularDamping();
}

void BulletRigidbody::SetAngularDamping(float damping)
{
	m_btRigidbody->setDamping(m_btRigidbody->getLinearDamping(), damping);
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

	BulletRigidbody* rb = (BulletRigidbody*)m_btRigidbody->getUserPointer();
	RigidbodyData* data = rb->GetData();
	retTransform.Scale = data->EntityID.GetTransform().Scale;

	return retTransform;
}

void BulletRigidbody::SetTransform(Math::Transform transform)
{
	btTransform retTransform;
	retTransform.setOrigin({transform.Position.x, transform.Position.y, transform.Position.z });
	retTransform.setRotation(btQuaternion(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z));
}
