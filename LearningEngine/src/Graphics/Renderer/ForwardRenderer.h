#pragma once

#include "API/RendererAPI.h"
#include "Mesh.h"
#include "Model.h"


class ForwardRenderer
{
public:
	static void Init(RendererAPI* rendererapi);
	static void BeginScene(PerspectiveCamera& camera);
	static void EndScene();
	static void Present();

	static void SubmitMesh(Mesh* mesh, glm::mat4 transform);
	static void SubmitModel(Model* model, glm::mat4 transform);
};

