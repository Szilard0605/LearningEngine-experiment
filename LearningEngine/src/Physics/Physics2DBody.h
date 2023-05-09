#pragma once

#include "glm.hpp"

class b2Body;
class b2BodyDef;
struct b2FixtureDef;

enum class BodyType
{
	Static,
	Kinematic,
	Dynamic

};

enum class Physics2DShape
{
	Quad, Circle
};

class Physics2DBody
{
public:
	Physics2DBody(glm::vec2 bPoisition, glm::vec2 bSize, float bRotation, BodyType bBodyType, Physics2DShape bShape);
	Physics2DBody(glm::vec2 bPoisition, float radius, float bRotation, BodyType bBodyType, Physics2DShape bShape);


	void SetPosition(const glm::vec2 position);
	glm::vec2 GetPosition();
	float GetRotation();
	void SetRotation(float rotation);
	glm::vec2 GetSize();
	void SetAwake(bool awake);
	
	float GetRadius();
	float SetRadius();

	Physics2DShape GetShape() { return m_shape; }
	b2Body* GetBody() { return m_body; }
	b2BodyDef* GetBodyDef() { return m_bodydef; }
	BodyType GetBodyType() { return m_bodytype; }
	Physics2DShape GetBodyShape() { return m_shape; }

	bool IsInContactWith(Physics2DBody& OtherBody);

	float GetDensity();
	void SetDensity(float density);
	
	float GetRestitution();
	void SetRestitution(float restitution);

	glm::vec2 GetWorldCenter();
	glm::vec2 GetWorldPoint(const glm::vec2& point);

	inline void SetBody(b2Body* body) { m_body = body; }
	inline void SetBodyDef(b2BodyDef* bodydef) { m_bodydef = bodydef; }

	void ApplyForce(const glm::vec2 force, const glm::vec2 point, const bool wake);
	void ApplyLinearImpulse(const glm::vec2 impulse, const glm::vec2 point, const bool wake);

	inline b2FixtureDef* GetFixtureDef() { return m_fixturedef; }

private:
	b2Body* m_body = nullptr;
	b2BodyDef* m_bodydef = nullptr;
	b2FixtureDef* m_fixturedef = nullptr;

	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation = 0;
	float m_radius = 0;

	BodyType m_bodytype;
	Physics2DShape m_shape;
};