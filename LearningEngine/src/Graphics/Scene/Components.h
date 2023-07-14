#pragma once

#include "glm.hpp"

#include "Graphics/Renderer/Model.h"

struct BaseComponent
{
	bool enabled = true;

};

struct TagComponent : public BaseComponent
{
	const char* ID = "TagComponent";

	std::string Tag;
};

struct TransformComponent : public BaseComponent
{
	const char* ID = "TransformComponent";

	glm::vec3 Position = {0, 0, 0};
	glm::vec3 Size = {1, 1, 1};
	glm::vec3 Rotation = {0, 0, 0};
};

struct QuadRendererComponent : public BaseComponent
{
	const char* ID = "QuadRendererComponent";

	glm::vec3 Scale = { 1, 1, 1 };
	glm::vec4 Color = {1, 1, 1, 1};
};

struct PerspectiveCameraComponent : public BaseComponent
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

struct StaticModelComponent : public BaseComponent
{
	const char* ID = "StaticModelComponent";

	Model* StaticModel = nullptr;
};