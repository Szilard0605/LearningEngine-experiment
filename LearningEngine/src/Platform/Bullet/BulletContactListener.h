#pragma once

#include <btBulletDynamicsCommon.h>

class BulletContactListener : public btCollisionWorld::ContactResultCallback
{
public:
    virtual btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0, int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1, int index1) override;
};

