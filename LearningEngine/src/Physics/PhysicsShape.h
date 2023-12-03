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
	float Radius;
};

struct BoxShape : public RigidbodyShape
{
	glm::vec3 Extents;
};

