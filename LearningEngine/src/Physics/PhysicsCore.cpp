#include "PhysicsCore.h"

#include "Platform/Bullet/BulletCore.h"


PhysicsCore* PhysicsCore::Create(PhysicsAPI api)
{
	switch (api)
	{
		case PhysicsAPI::Bullet:
		{
			PhysicsCore* core = new BulletCore();
			core->SetAPI(api);
			return core;
		}
	}
	return nullptr;
}
