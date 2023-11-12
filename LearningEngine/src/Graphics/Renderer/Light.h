#pragma once

#include "glm.hpp"

struct Light
{
	glm::vec3 Color;
	float Intensity;
};

struct PointLight : public Light
{
	glm::vec3 Position;
};

struct DirectionalLight : public Light
{
	glm::vec3 Direction;
};
