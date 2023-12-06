#pragma once

#include "API/RendererAPI.h"
#include "Mesh.h"
#include "Model.h"

#include "Light.h"

#define MAX_LIGHTS 100

class ForwardRenderer
{
public:
	struct RenderStatistics
	{
		uint32_t DrawCalls;
		uint32_t MeshCount;
		uint32_t Vertices;
		uint32_t PointLightCount;
		uint32_t DirectionalLightCount;
	};

	static void Init(RendererAPI* rendererapi);
	static void BeginScene(PerspectiveCamera& camera);
	static void EndScene();
	static void Present();

	static void SubmitMesh(Mesh& mesh, glm::mat4 transform, int entity = -1);
	static void SubmitModel(Model& model, glm::mat4 transform, int entity = -1);


	static void SubmitLight(AmbientLight& light);

	static void SubmitLight(PointLight& light);
	static void SubmitLight(DirectionalLight& light);

	static RenderStatistics& GetRenderStatistics();

};

