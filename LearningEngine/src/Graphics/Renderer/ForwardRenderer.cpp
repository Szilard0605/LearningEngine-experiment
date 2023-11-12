#include "ForwardRenderer.h"
#include "ForwardRenderer.h"
#include "ShaderLibrary.h"

#include "API/ShaderBuffer.h"

#include <vector>
#include <gtc/type_ptr.hpp>
#include <glew.h>

struct MeshRenderData
{
	Mesh* mesh;
	glm::mat4 transform;
};

struct LightData
{
	glm::vec4 Color;
	glm::vec4 Position; // X, Y, Z, TYPE
	glm::vec4 Direction;
};


struct LightBufferData
{
	std::vector<LightData> Lights;
};

struct RenderDataSB
{
	uint64_t NumLights;
	uint64_t Padding;
};

struct RenderData
{
	ShaderBuffer* RenderDataBuffer;
	RenderDataSB DataBuffer;
	
	std::vector<MeshRenderData> meshes;
	Shader* shader;
	PerspectiveCamera camera;

	LightBufferData LightData;
	ShaderBuffer* LightBuffer;

};

static RenderData s_RenderData;
static ForwardRenderer::RenderStatistics s_RenderStats;

void ForwardRenderer::Init(RendererAPI* rendererapi)
{
	s_RenderData.shader = ShaderLibrary::Load("ForwardRenderer", "res/shaders/default_shader.shader");


	s_RenderData.LightBuffer = ShaderBuffer::Create(sizeof(LightData) * MAX_LIGHTS);
	s_RenderData.LightData.Lights.reserve(MAX_LIGHTS);

	
	s_RenderData.RenderDataBuffer = ShaderBuffer::Create(sizeof(RenderDataSB));
}

void ForwardRenderer::BeginScene(PerspectiveCamera& camera)
{
	s_RenderData.camera = camera;
	
	s_RenderData.meshes.clear();

	s_RenderData.LightData.Lights.clear();

	s_RenderData.DataBuffer.NumLights = 0;

	s_RenderStats.DrawCalls = 0;
	s_RenderStats.MeshCount = 0;
	s_RenderStats.Vertices = 0;
	s_RenderStats.PointLightCount = 0;
}

void ForwardRenderer::EndScene()
{
	
}

void ForwardRenderer::Present()
{

	for (int i = 0; i < s_RenderData.meshes.size(); i++)
	{
		s_RenderStats.DrawCalls++;
		
		
		s_RenderData.shader->Bind();


		s_RenderData.RenderDataBuffer->Bind(0);
		s_RenderData.RenderDataBuffer->SetData(&s_RenderData.DataBuffer, sizeof(RenderDataSB), 0);


		// Light setup
		{
			s_RenderData.LightBuffer->Bind(1);
			s_RenderData.LightBuffer->SetData(s_RenderData.LightData.Lights.data(), sizeof(LightData) * MAX_LIGHTS, 0);

		}

		s_RenderData.meshes[i].mesh->Render(s_RenderData.camera, s_RenderData.meshes[i].transform);
	}
}

void ForwardRenderer::SubmitMesh(Mesh* mesh, glm::mat4 transform)
{
	s_RenderStats.MeshCount++;
	s_RenderStats.Vertices += mesh->GetVertices().size();

	s_RenderData.meshes.emplace_back(MeshRenderData{ mesh,  transform });
}

void ForwardRenderer::SubmitModel(Model* model, glm::mat4 transform)
{
	for (int i = 0; i < model->GetMeshes().size(); i++)
	{
		SubmitMesh(model->GetMeshes()[i], transform);
	}
}
void ForwardRenderer::SubmitLight(PointLight& light)
{
	LightData plData;
	plData.Position = glm::vec4(light.Position, 0.0f);
	plData.Color = glm::vec4(light.Color, light.Intensity);
	s_RenderData.LightData.Lights.push_back(plData);

	s_RenderData.DataBuffer.NumLights++;
	s_RenderStats.PointLightCount++;
}

void ForwardRenderer::SubmitLight(DirectionalLight& light)
{
	LightData dlData;
	dlData.Position.w = 1.0f;
	dlData.Direction = glm::vec4(light.Direction, 1.0f);
	dlData.Color = glm::vec4(light.Color, light.Intensity);
	s_RenderData.LightData.Lights.push_back(dlData);

	s_RenderData.DataBuffer.NumLights++;
	s_RenderStats.DirectionalLightCount++;
}

ForwardRenderer::RenderStatistics& ForwardRenderer::GetRenderStatistics()
{
	return s_RenderStats;
}

