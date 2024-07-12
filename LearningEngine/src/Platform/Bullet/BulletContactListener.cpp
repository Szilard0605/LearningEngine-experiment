#include "BulletContactListener.h"

#include "Log/Log.h"
#include "Graphics/Scene/Entity.h"
#include <Events/PhysicsEvents.h>

btScalar BulletContactListener::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
    const btCollisionObject* rb0 = colObj0Wrap->getCollisionObject();
    const btCollisionObject* rb1 = colObj1Wrap->getCollisionObject();

    RigidbodyData* rbData0 = (RigidbodyData*)rb0->getUserPointer();
    RigidbodyData* rbData1 = (RigidbodyData*)rb1->getUserPointer();

    Entity entity0((entt::entity)rbData0->EntityID, rbData0->Scene);
    Entity entity1((entt::entity)rbData1->EntityID, rbData1->Scene);

    if (!entity0.IsValid() || !entity1.IsValid())
    {
        LE_CORE_ERROR("Collision with invalid entity (or entites) %d and %d", entity0.GetHandle(), entity1.GetHandle());
        return 0;
    }
    
    PhysicsContactEvent contactEvent(entity0.GetHandle(), entity1.GetHandle());
    m_ContactCallback(contactEvent);
    return 0;
}
