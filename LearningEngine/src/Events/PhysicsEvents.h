#pragma once

#include "Event.h"

#include "entt.hpp"

class PhysicsContactEvent : public Event
{
protected:
	entt::entity m_EntityA = entt::null;
	entt::entity m_EntityB = entt::null;
public:
	PhysicsContactEvent(entt::entity entityHandleA, entt::entity entityHandleB)
		: m_EntityA(entityHandleA), m_EntityB(entityHandleB)
	{
		//m_Scene = entityA.GetScene();
	}

	EventType GetType() const override { return EventType::PHYSICS_CONTACT; }
	static EventType GetStaticType()   { return EventType::PHYSICS_CONTACT; }
	entt::entity GetEntityHandleA()    { return m_EntityA; }
	entt::entity GetEntityHandleB()	   { return m_EntityB; }
};