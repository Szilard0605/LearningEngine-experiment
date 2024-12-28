#pragma once

#include "API/RendererAPI.h"
#include "Mesh.h"
#include "Model.h"

#include "Light.h"

#define MAX_LIGHTS 100
#define MAX_MESHES 20000

class ForwardRenderer
{
public:
	struct RenderStatistics
	{
		uint32_t DrawCalls = 0;
		uint32_t MeshCount = 0;
		uint32_t TotalVertices = 0;
		uint32_t PointLightCount = 0;
		uint32_t DirectionalLightCount = 0;


		double CPURenderTime = .0f;
	};

	static void Init(RendererAPI* rendererapi);
	static void BeginScene(PerspectiveCamera& camera);
	static void EndScene();
	static void Present(Framebuffer* FrameBuffer);

	static void SubmitMesh(Mesh& mesh, glm::mat4 transform, int entity = -1);
	static void SubmitModel(Model& model, glm::mat4 transform, int entity = -1);
	static void SetSkybox(CubeMap* cubeMap);
	static void DBGOrtho(float nearPlane, float farPlane, float left = -10.0f, float right = 10.0f, float bottom = -10.0f, float top = 10.0f, float lightDist = 124.0f, float sceneSize = 10.f);

	static uint32_t GetDepthMapDBG();

	static void SubmitLight(AmbientLight& light);

	static void SubmitLight(PointLight& light);
	static void SubmitLight(DirectionalLight& light);

	static RenderStatistics& GetRenderStatistics();

};

