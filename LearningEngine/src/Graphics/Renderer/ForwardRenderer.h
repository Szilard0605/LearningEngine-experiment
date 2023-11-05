#pragma once

#include "API/RendererAPI.h"
#include "Mesh.h"
#include "Model.h"

class ForwardRenderer
{
public:
	struct RenderStatistics
	{
		uint32_t DrawCalls;
		uint32_t MeshCount;
		uint32_t Vertices;
	};

	static void Init(RendererAPI* rendererapi);
	static void BeginScene(PerspectiveCamera& camera);
	static void EndScene();
	static void Present();


	static void SubmitMesh(Mesh* mesh, glm::mat4 transform);
	static void SubmitModel(Model* model, glm::mat4 transform);

	static RenderStatistics& GetRenderStatistics();

};

