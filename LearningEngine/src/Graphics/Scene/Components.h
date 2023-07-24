#pragma once

#include "glm.hpp"
#include <variant>

#include "Graphics/Renderer/Model.h"

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

struct PerspectiveCameraComponent
{
	const char* ID = "PerspectiveCameraComponent";

	PerspectiveCamera* Camera = nullptr;
	
	bool MainCamera = false;;
	
	glm::vec3 FocalPoint = { 0.0f, 0.0f, 0.0f };
	float Distance = 10.0f;
	float Pitch = 0.0f;
    float Yaw = 0.0f;
	float FOV = 60.0f;
	float AspectRatio = 1280.0f / 720.0f;
	bool FixedAspectRatio = false;
	float NearClip = 1.0f;
	float FarClip = 10000.0f;
};

struct StaticModelComponent
{
	const char* ID = "StaticModelComponent";

	Model* StaticModel = nullptr;
};

template <typename... Component>
struct Components
{

};

using EveryComponent = Components<TransformComponent, TagComponent, QuadRendererComponent, PerspectiveCameraComponent, StaticModelComponent>;