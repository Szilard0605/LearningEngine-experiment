#pragma once

#include "glm.hpp"

enum class ShapeType
{
	None = 0,
	Box,
	Sphere,
	Triangle,
	Convex
};


struct RigidbodyShape
{
	ShapeType Shape = ShapeType::None;
};

struct SphereShape : public RigidbodyShape
{
	SphereShape(float radius)
		: Radius(radius) {}

	float Radius;
};

struct BoxShape : public RigidbodyShape
{
	BoxShape(glm::vec3 size)
		: Size(size) { }

	glm::vec3 Size = { 1, 1, 1 };
};

