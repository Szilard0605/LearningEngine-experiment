#pragma once

#include "glm.hpp"

struct TagComponent
{
	std::string Tag;
};

struct TransformComponent
{
	const char* Name = "Transform Component";
	glm::vec3 Position = {0, 0, 0};
	glm::vec3 Size = {1, 1, 1};
};

struct QuadRendererComponent
{
	const char* Name = "Quad Renderer Component";
	glm::vec2 Position;
	glm::vec2 Size;
};