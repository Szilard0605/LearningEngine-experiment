#pragma once

#include "glm.hpp"

struct TransformComponent
{
	glm::vec3 Position = {0, 0, 0};
	glm::vec3 Size = {1, 1, 1};
};