#include "PhysicsWorld.h"

#include "Platform/Bullet/BulletWorld.h"

#include "Core/Application.h"

#include "Graphics/Scene/Entity.h"

PhysicsWorld* PhysicsWorld::Create(Scene* scene, glm::vec3 gravity)
{
    PhysicsCore* core = Application::GetInstance()->GetPhysicsCore();
    
    switch (core->GetAPI())
    {
        case PhysicsAPI::Bullet:
        {
            return new BulletWorld(scene, gravity);
        }
    }
    
    return nullptr;
}
