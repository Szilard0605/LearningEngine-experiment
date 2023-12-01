#include "PhysicsCore.h"

#include "Platform/Bullet/BulletCore.h"


PhysicsCore* PhysicsCore::Create(PhysicsAPI api)
{
	switch (api)
	{
		case PhysicsAPI::Bullet:
		{
			return new BulletCore();
		}
	}
	return nullptr;
}
