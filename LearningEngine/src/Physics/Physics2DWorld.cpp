#include "Physics2DWorld.h"
#include <cinttypes>

#include "box2d/box2d.h"

Physics2DWorld::Physics2DWorld(const glm::vec2& gravity)
	: m_gravity(gravity)
{
	m_world = new b2World(b2Vec2(gravity.x, gravity.y));
}

void Physics2DWorld::AddBody(Physics2DBody& body)
{
	b2BodyDef* bodydef;
	bodydef = new b2BodyDef();
	body.SetBodyDef(bodydef);

	bodydef->type = (b2BodyType)body.GetBodyType();
	bodydef->position.Set(body.GetPosition().x, body.GetPosition().y);
	bodydef->angle = body.GetRotation();

	b2Body* worldbody;
	worldbody = m_world->CreateBody(bodydef);
	body.SetBody(worldbody);


	switch (body.GetShape())
	{
		case Physics2DShape::Quad:
		{	
			b2PolygonShape shape;
			shape.SetAsBox(body.GetSize().x / 2 , body.GetSize().y / 2);
			body.GetFixtureDef()->shape = &shape;
			b2Fixture* fixture = worldbody->CreateFixture(body.GetFixtureDef());
			break;
		}

		case Physics2DShape::Circle:
		{
			b2CircleShape shape;
			shape.m_p.Set(0, 0);
			shape.m_radius = body.GetRadius();
			body.GetFixtureDef()->shape = &shape;
			b2Fixture* fixture = worldbody->CreateFixture(body.GetFixtureDef());
			break;
		}
	}
}

void Physics2DWorld::Step(float timestep)
{
	m_world->Step(timestep, m_VelocityIterations, m_PositionIterations);
}
