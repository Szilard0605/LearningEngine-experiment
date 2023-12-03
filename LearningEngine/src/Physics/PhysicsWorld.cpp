#include "PhysicsWorld.h"

#include "Platform/Bullet/BulletWorld.h"

#include "Core/Application.h"

PhysicsWorld* PhysicsWorld::Create(glm::vec3 gravity)
{
    PhysicsCore* core = Application::GetInstance()->GetPhysicsCore();
    
    switch (core->GetAPI())
    {
        case PhysicsAPI::Bullet:
        {
            return new BulletWorld(gravity);
        }
    }
    
    return nullptr;
}
