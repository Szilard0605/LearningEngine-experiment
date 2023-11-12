#pragma once

#include "Graphics/Renderer/Model.h"


#include "glm.hpp"
#include <vector>


struct HierarchyComponent
{
	entt::entity Parent = entt::null;
	std::vector<entt::entity> Children;
};

struct BaseComponent
{
	bool enabled = true;
};

struct TagComponent : public BaseComponent
{
	int ID = 0;

	std::string Tag;
};

struct TransformComponent : public BaseComponent
{
	const char* ID = "TransformComponent";

	glm::vec3 Position = {0, 0, 0};
	glm::vec3 Scale = {1, 1, 1};
	glm::vec3 Rotation = {0, 0, 0};

	glm::mat4 GetTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

		return glm::translate(glm::mat4(1.0f), Position)
			* rotation
			* glm::scale(glm::mat4(1.0f), Scale);
	}
};

struct QuadRendererComponent : public BaseComponent
{
	const char* ID = "QuadRendererComponent";

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

	// TODO (Szilard): We HAVE TO change this to store Model Asset handle (when asset system done)
	Model* StaticModel = nullptr;
};

struct PointLightComponent : public BaseComponent
{
	const char* ID = "PointLightComponent";

	float Intensity = 1.0f;
	glm::vec4 Color;
};

struct DirectionalLightComponent : public BaseComponent
{
	const char* ID = "DirectionalLightComponent";

	float Intensity;
	glm::vec3 Direction;
};

template <typename... Component>
struct Components
{

};

using EveryComponent = Components<TransformComponent, TagComponent, HierarchyComponent, QuadRendererComponent, PerspectiveCameraComponent, StaticModelComponent>;