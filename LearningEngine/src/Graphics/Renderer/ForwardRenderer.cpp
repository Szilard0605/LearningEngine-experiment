#include "ForwardRenderer.h"
#include "ForwardRenderer.h"
#include "ShaderLibrary.h"

#include <vector>

struct MeshRenderData
{
	Mesh* mesh;
	glm::mat4 transform;
};

struct RenderData
{
	std::vector<MeshRenderData> meshes;
	Shader* shader;
	PerspectiveCamera camera;
};

static RenderData s_RenderData;

void ForwardRenderer::Init(RendererAPI* rendererapi)
{
	s_RenderData.shader = ShaderLibrary::Load("ForwardRenderer", "res/shaders/default_shader.shader");
}

void ForwardRenderer::BeginScene(PerspectiveCamera& camera)
{
	s_RenderData.camera = camera;
	s_RenderData.meshes.clear();
}

void ForwardRenderer::EndScene()
{
}

void ForwardRenderer::Present()
{
	for (int i = 0; i < s_RenderData.meshes.size(); i++)
	{
		s_RenderData.meshes[i].mesh->Render(s_RenderData.camera, s_RenderData.meshes[i].transform);
	}
}

void ForwardRenderer::SubmitMesh(Mesh* mesh, glm::mat4 transform)
{
	s_RenderData.meshes.emplace_back(MeshRenderData{ mesh,  transform });
}

void ForwardRenderer::SubmitModel(Model* model, glm::mat4 transform)
{
	for (int i = 0; i < model->GetMeshes().size(); i++)
	{
		SubmitMesh(model->GetMeshes()[i], transform);
	}
}
