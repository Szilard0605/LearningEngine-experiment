#pragma once

#include "glm.hpp"

struct TagComponent
{
	std::string Tag;
};

struct TransformComponent
{
	glm::vec3 Position = {0, 0, 0};
	glm::vec3 Size = {1, 1, 1};
	glm::vec3 Rotation = {0, 0, 0};
};

struct QuadRendererComponent
{
	glm::vec3 Scale = {1, 1, 1};
	glm::vec4 Color = {1, 1, 1, 1};
};