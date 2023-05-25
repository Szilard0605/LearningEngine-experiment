#pragma once

#include "glm.hpp"

struct TagComponent
{
	const char* ID = "TagComponent";

	std::string Tag;
};

struct TransformComponent
{
	const char* ID = "TransformComponent";

	glm::vec3 Position = {0, 0, 0};
	glm::vec3 Size = {1, 1, 1};
	glm::vec3 Rotation = {0, 0, 0};
};

struct QuadRendererComponent
{
	const char* ID = "QuadRendererComponent";

	glm::vec3 Scale = {1, 1, 1};
	glm::vec4 Color = {1, 1, 1, 1};
};