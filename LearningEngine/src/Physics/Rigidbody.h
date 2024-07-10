#pragma once

#include "PhysicsShape.h"

#include "Math/Transform.h"

#include "entt.hpp"

#include "Graphics/Scene/Scene.h"

class Entity;

struct RigidbodyData
{
	uint32_t EntityID;
	Scene* Scene;
};

class Rigidbody
{
public:

	static Rigidbody* Create(Entity entity, SphereShape shape);
	static Rigidbody* Create(Entity entity, BoxShape shape);
	//static Rigidbody* Create(Math::Transform& transform, SphereShape& shape);
	//static Rigidbody* Create(Math::Transform& transform, BoxShape& shape);

	virtual entt::entity GetEntityHandle() = 0;

	virtual void WakeUp(bool forceWakeUp) = 0;
	virtual void Sleep() = 0;

	virtual float GetMass() = 0;
	virtual void SetMass(float mass) = 0;

	virtual float GetLinearDamping() = 0;
	virtual void SetLinearDamping(float damping) = 0;
	virtual float GetAngularDamping() = 0;
	virtual void SetAngularDamping(float damping) = 0;
	virtual void ApplyForce(glm::vec3 force, glm::vec3 relativePos) = 0;
	virtual void ApplyCentralForce(glm::vec3 force) = 0;

	virtual void SetShape(BoxShape shape) = 0;
	virtual void SetShape(SphereShape shape) = 0;
	virtual Math::Transform GetTransform() = 0;
	virtual void SetTransform(Math::Transform transform) = 0;

};

