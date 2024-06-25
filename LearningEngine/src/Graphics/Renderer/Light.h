#pragma once

#include "glm.hpp"

struct AmbientLight
{
	glm::vec3 Color;
	float Intensity;
	float SpecularPower;
};

struct Light
{
	glm::vec3 Color;
	float Intensity;
	float SpecularPower;
};

struct PointLight : public Light
{
	glm::vec3 Position;
};

struct DirectionalLight : public Light
{
	glm::vec3 Direction;
};
