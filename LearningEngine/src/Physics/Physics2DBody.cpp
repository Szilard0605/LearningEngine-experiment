#include "Physics2DBody.h"

#include "box2d/box2d.h"

Physics2DBody::Physics2DBody(glm::vec2 bPoisition, glm::vec2 bSize, float bRotation, BodyType bBodyType, Physics2DShape bShape)
	: m_position(bPoisition),  m_size(bSize), m_rotation(bRotation), m_bodytype(bBodyType), m_shape(bShape)
{
	m_fixturedef = new b2FixtureDef();
}

Physics2DBody::Physics2DBody(glm::vec2 bPoisition, float radius, float bRotation, BodyType bBodyType, Physics2DShape bShape)
	: m_position(bPoisition), m_radius(radius), m_rotation(bRotation), m_bodytype(bBodyType), m_shape(bShape)
{
	m_fixturedef = new b2FixtureDef();
}

void Physics2DBody::SetPosition(const glm::vec2 position)
{
	if (m_body)
	{
		m_body->SetTransform(b2Vec2(position.x, position.y), m_body->GetAngle());
	}
	else
	{
		m_position = position;
	}
}

glm::vec2 Physics2DBody::GetPosition()
{

	if (m_body)
	{
		return { m_body->GetPosition().x, m_body->GetPosition().y };
	}
	else
	{
		return m_position;
	}
}

void Physics2DBody::SetAwake(bool awake)
{
	m_body->SetAwake(awake);
}


float Physics2DBody::GetRadius()
{
	if (m_fixturedef->shape)
	{
		return m_fixturedef->shape->m_radius; 
	}
	else
	{
		return m_radius;
	}
}

float Physics2DBody::SetRadius()
{
	return 0.0f;
}

float Physics2DBody::GetRotation()
{
	if (m_body)
		return m_body->GetAngle();
	else
		return m_rotation;
}

bool Physics2DBody::IsInContactWith(Physics2DBody& OtherBody)
{
	b2ContactEdge* contactedge = m_body->GetContactList();

	if (contactedge && contactedge->contact->GetFixtureB()->GetBody() == OtherBody.GetBody())
		return true;
	else
		return false;
}

void Physics2DBody::SetRotation(float rotation)
{
	//rotation *= 0.0174532925; // DEG TO RAD

	if (m_body)
		m_body->SetTransform(m_body->GetPosition(), rotation *= 0.0174532925);
	else
		m_rotation = rotation;
}

glm::vec2 Physics2DBody::GetSize()
{
	return m_size;
}

float Physics2DBody::GetDensity()
{
	return m_fixturedef->density;
}

void Physics2DBody::SetDensity(float density)
{
	m_fixturedef->density = density;
}

float Physics2DBody::GetRestitution()
{
	return m_fixturedef->restitution;
}

void Physics2DBody::SetRestitution(float restitution)
{
	m_fixturedef->restitution = restitution;
}

glm::vec2 Physics2DBody::GetWorldCenter()
{
	b2Vec2 WorldCenter = m_body->GetWorldCenter();
	return glm::vec2(WorldCenter.x, WorldCenter.y);
}

glm::vec2 Physics2DBody::GetWorldPoint(const glm::vec2& point)
{
	b2Vec2 WorldPoint = m_body->GetWorldPoint(b2Vec2(point.x, point.y));
	return glm::vec2(WorldPoint.x, WorldPoint.y);
}

void Physics2DBody::ApplyForce(const glm::vec2 force, const glm::vec2 point, const bool wake)
{
	m_body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), wake);
}

void Physics2DBody::ApplyLinearImpulse(const glm::vec2 impulse, const glm::vec2 point, const bool wake)
{
	m_body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), wake); 
}
