#include "BulletWorld.h"
#include "BulletCore.h"
#include "Core/Application.h"

#include "btBulletDynamicsCommon.h"
#include "BulletRigidbody.h"

#include "Graphics/Scene/Entity.h"

struct RigidbodyData
{
	Entity EntityID;
};

BulletWorld::BulletWorld(Scene* scene, glm::vec3 gravity)
	: m_Scene(scene)
{
	BulletCore* core = (BulletCore*)Application::GetInstance()->GetPhysicsCore();
	m_btWorld = new btDiscreteDynamicsWorld(core->GetCollisionDispatcher(), core->GetBroadphaseInterface(), core->SequentialImpulseConstraintSolver(), core->GetCollisionConfiguration());
	m_btWorld->setGravity({ gravity.x, gravity.y, gravity.z });

	// Box colliders
	{
		auto view = m_Scene->Registry.view<RigidbodyComponent, BoxColliderComponent>();
		for (auto entity : view)
		{
			auto [rc, bcc] = view.get<RigidbodyComponent, BoxColliderComponent>(entity);
			TransformComponent& tc = m_Scene->Registry.get<TransformComponent>(entity);
			BulletRigidbody rb(Entity(entity, m_Scene), BoxShape(bcc.Size));
			m_Rigidbodies.push_back(rb);
		}
	}

	// Sphere colliders
	{
		auto view = m_Scene->Registry.view<RigidbodyComponent, SphereColliderComponent>();
		for (auto entity : view)
		{
			auto [rc, scc] = view.get<RigidbodyComponent, SphereColliderComponent>(entity);
			TransformComponent& tc = m_Scene->Registry.get<TransformComponent>(entity);
			BulletRigidbody rb(Entity(entity, m_Scene), SphereShape(scc.Radius));
			m_Rigidbodies.push_back(rb);
		}
	}

	for (int i = 0; i < m_Rigidbodies.size(); i++)
	{
		m_btWorld->addRigidBody(m_Rigidbodies[i].GetBulletRigidbody());
	}
}

BulletWorld::~BulletWorld()
{
	m_Rigidbodies.clear();
	delete m_btWorld;
}


void BulletWorld::AddRigidBody(Rigidbody* body)
{
	BulletRigidbody* bulletBody = (BulletRigidbody*)&body;
	m_Rigidbodies.push_back(*bulletBody);

	BulletRigidbody test = m_Rigidbodies[m_Rigidbodies.size()-1];
	printf("entity: %d\n", test.GetData()->EntityID.GetHandle());

	m_btWorld->addRigidBody(bulletBody->GetBulletRigidbody());
}

void BulletWorld::StepSimulation(float timeStep)
{
	m_btWorld->stepSimulation(timeStep, 10);

	for (int i = 0; i < m_Rigidbodies.size(); i++)
	{
		BulletRigidbody rigidBody = m_Rigidbodies[i];
		btRigidBody* btRigidBody = rigidBody.GetBulletRigidbody();
		Entity entity = rigidBody.GetData()->EntityID;

		btTransform& simTransform = btRigidBody->getWorldTransform();
		entity.SetPosition({ simTransform.getOrigin().x(), simTransform.getOrigin().y(), simTransform.getOrigin().z() });

		float yaw, pitch, roll;
		simTransform.getBasis().getEulerZYX(yaw, pitch, roll);
		entity.SetRotation({ roll, pitch, yaw });

		RigidbodyComponent& rc = entity.GetComponent<RigidbodyComponent>();
		btRigidBody->setMassProps(rc.Mass, btRigidBody->getLocalInertia());
		btRigidBody->setDamping(rc.LinearDamping, rc.AngularDamping);
	}
}

void BulletWorld::DestroyAllRigidbodies()
{
	for (int i = m_btWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		m_btWorld->removeCollisionObject(m_btWorld->getCollisionObjectArray()[i]);
	}
}

