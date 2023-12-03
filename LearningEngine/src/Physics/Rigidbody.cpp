#include "Rigidbody.h"

#include "Core/Application.h"
#include "PhysicsCore.h"

#include "Platform/Bullet/BulletRigidbody.h"

Rigidbody* Rigidbody::Create(Math::Transform& transform, SphereShape& shape)
{
    PhysicsCore* core = Application::GetInstance()->GetPhysicsCore();

    switch (core->GetAPI())
    {
        case PhysicsAPI::Bullet:
        {
            return new BulletRigidbody(transform, shape);
        }
    }
	return nullptr;
}

Rigidbody* Rigidbody::Create(Math::Transform& transform, BoxShape& shape)
{
    PhysicsCore* core = Application::GetInstance()->GetPhysicsCore();

    switch (core->GetAPI())
    {
        case PhysicsAPI::Bullet:
        {
            return new BulletRigidbody(transform, shape);
        }
    }
    return nullptr;
}