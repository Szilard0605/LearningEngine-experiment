#pragma once

#include <btBulletDynamicsCommon.h>

#include "Events/PhysicsEvents.h"

#include <functional>

class BulletContactListener : public btCollisionWorld::ContactResultCallback
{
public:
    virtual btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0, int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1, int index1) override;

    inline void SetContactCallback(std::function<void(PhysicsContactEvent&)> callback) { m_ContactCallback = callback; }

private:
    std::function<void(PhysicsContactEvent&)> m_ContactCallback;
};

