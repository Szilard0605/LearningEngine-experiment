#pragma once

enum PhysicsAPI
{
	Bullet
};

class PhysicsCore
{
public:
	static PhysicsCore* Create(PhysicsAPI api);
	
	PhysicsAPI GetAPI() { return m_API; }
	void SetAPI(PhysicsAPI api) { m_API = api; }
private:
	PhysicsAPI m_API;
};

