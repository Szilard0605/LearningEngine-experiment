#pragma once

enum PhysicsAPI
{
	Bullet
};

class PhysicsCore
{
public:
	static PhysicsCore* Create(PhysicsAPI api);
};

