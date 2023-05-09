
#include "glm.hpp"
#include "vector"

#include "Physics2DBody.h"

class b2World;

class Physics2DWorld
{
public:
	Physics2DWorld(const glm::vec2& gravity);
	void AddBody(Physics2DBody& body);
	void Step(float timestep);

private:
	glm::vec2 m_gravity;
	b2World* m_world;
	int32_t m_VelocityIterations = 6;
	int32_t m_PositionIterations = 2;
};