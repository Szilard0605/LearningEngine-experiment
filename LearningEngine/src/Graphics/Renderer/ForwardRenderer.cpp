#include "ForwardRenderer.h"
#include "ForwardRenderer.h"
#include "ShaderLibrary.h"

#include "Core/Application.h"
#include "API/ShaderBuffer.h"

#include <vector>
#include <gtc/type_ptr.hpp>
#include <glew.h>
#include <imgui.h>

struct MeshRenderData
{
	Mesh mesh;
	glm::mat4 transform;
	int EntityID;
};

struct LightData
{
	glm::vec4 Color;
	glm::vec4 Position; // X, Y, Z, TYPE
	glm::vec4 Direction; // X, Y, Z, SpecularPower
};


struct LightBufferData
{
	std::vector<LightData> Lights;
};

struct RenderDataSB
{
	glm::vec4 AmbientLight;
	uint64_t NumLights;
	uint64_t Padding;
	glm::vec4 CameraPosition;
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

	Framebuffer* DepthMapFB;
	Shader* DepthMapShader;
};

static RenderData s_RenderData;
static ForwardRenderer::RenderStatistics s_RenderStats;

void ForwardRenderer::Init(RendererAPI* rendererapi)
{
	s_RenderData.shader = ShaderLibrary::Load("ForwardRenderer", "res/shaders/default_shader.shader");


	s_RenderData.LightBuffer = ShaderBuffer::Create(sizeof(LightData) * MAX_LIGHTS);
	s_RenderData.LightData.Lights.reserve(MAX_LIGHTS);

	
	s_RenderData.RenderDataBuffer = ShaderBuffer::Create(sizeof(RenderDataSB));


	FramebufferSpecifications depthFBSpecs;
	depthFBSpecs.Attachments = {FramebufferAttachment::Depth };
	depthFBSpecs.Width = 1024;
	depthFBSpecs.Height = 1024;
	s_RenderData.DepthMapFB = Framebuffer::Create(depthFBSpecs);

	s_RenderData.DepthMapShader = Shader::Create("res/shaders/depthmap.shader");
}

void ForwardRenderer::BeginScene(PerspectiveCamera& camera)
{
	s_RenderData.camera = camera;


	s_RenderData.meshes.clear();
	s_RenderData.DataBuffer.CameraPosition = glm::vec4(camera.GetPosition(), 1.0f);
	
	s_RenderData.DataBuffer.NumLights = 0;
	s_RenderStats.DrawCalls = 0;
	s_RenderStats.MeshCount = 0;
	s_RenderStats.TotalVertices = 0;
	s_RenderStats.PointLightCount = 0;
	s_RenderStats.DirectionalLightCount = 0;
	s_RenderData.LightData.Lights.clear();
}

void ForwardRenderer::EndScene()
{

}

void ForwardRenderer::Present(Framebuffer* FrameBuffer)
{
	RendererAPI* api = Application::GetInstance()->GetRenderer();
	auto beginTime = std::chrono::high_resolution_clock::now();
	// directional light shadow map pass

	glm::mat4 lightSpaceMatrix;

	for (int i = 0; i < s_RenderData.LightData.Lights.size(); i++)
	{
		LightData& light = s_RenderData.LightData.Lights[i];

		if (s_RenderData.LightData.Lights[i].Position.w == 1)
		{
			glm::vec3 lightDirection = glm::normalize(glm::vec3(light.Direction)); // Light's direction (normalized)
			glm::vec3 lightPosition = -lightDirection * 1000.0f; // Place light far away from origin
			glm::vec3 lightTarget = glm::vec3(0.0f); // The target is the origin or the center of the scene
			glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector, usually the y-axis
			glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, upVector);
			
			// Set the orthographic projection matrix
			float nearPlane = 0.1f;
			float farPlane = 2000.0f;
			float left = -50.0f; // Adjust based on your scene
			float right = 50.0f;
			float bottom = -50.0f;
			float top = 50.0f;


			glm::mat4 lightProjection = glm::ortho(left, right, bottom, top, nearPlane, farPlane);

			// Combine them into the light space matrix
			glm::mat4 lightSpaceMatrix = lightProjection * lightView;
			lightSpaceMatrix = lightProjection * lightView;

			// Send lightSpaceMatrix to the shader
			s_RenderData.DepthMapShader->Bind();

			RendererAPI* api = Application::GetInstance()->GetRenderer();
			s_RenderData.DepthMapFB->Bind();
			api->SetViewportSize(1024, 1024);

			api->ClearDepthBuffer();
			s_RenderData.DepthMapShader->SetMatrix4f("u_LightSpaceMatrix", lightSpaceMatrix);
			
			for (int i = 0; i < s_RenderData.meshes.size(); i++)
			{
				s_RenderData.DepthMapShader->SetMatrix4f("u_Model", s_RenderData.meshes[i].transform);
				s_RenderStats.DrawCalls++;
				s_RenderData.meshes[i].mesh.Render(s_RenderData.camera, s_RenderData.meshes[i].transform, s_RenderData.meshes[i].EntityID);
			}
		}
	}
	
	s_RenderData.RenderDataBuffer->Bind(0);
	s_RenderData.RenderDataBuffer->SetData(&s_RenderData.DataBuffer, sizeof(RenderDataSB), 0);

	// Light setup
	s_RenderData.LightBuffer->Bind(1);
	s_RenderData.LightBuffer->SetData(s_RenderData.LightData.Lights.data(), sizeof(LightData) * MAX_LIGHTS, 0);
	
	s_RenderData.shader->Bind();
	//s_RenderData.shader->SetMatrix4f("u_LightSpaceMatrix", lightSpaceMatrix);
	
	glBindTextureUnit(1, s_RenderData.DepthMapFB->GetDepthAttachmentID());
	s_RenderData.shader->SetInt("u_DepthMap", s_RenderData.DepthMapFB->GetDepthAttachmentID());
	
	FrameBuffer->Bind();
	api->Clear({ 0.5, 0.5, 0.5, 1 });
	api->ClearDepthBuffer();
	for (int i = 0; i < s_RenderData.meshes.size(); i++)
	{
		s_RenderStats.DrawCalls++;
		s_RenderData.meshes[i].mesh.GetMaterial()->GetShader()->SetMatrix4f("u_ViewProjection", s_RenderData.camera.GetViewProjection());
		s_RenderData.meshes[i].mesh.GetMaterial()->GetShader()->SetMatrix4f("u_Transform", s_RenderData.meshes[i].transform);
		
		if (s_RenderData.meshes[i].mesh.GetMaterial()->GetTexture())
		{
			s_RenderData.meshes[i].mesh.GetMaterial()->GetTexture()->Bind(0);
		}

		s_RenderData.meshes[i].mesh.Render(s_RenderData.camera, s_RenderData.meshes[i].transform, s_RenderData.meshes[i].EntityID);
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> finalTime = endTime - beginTime;
	s_RenderStats.CPURenderTime = finalTime.count();
}

void ForwardRenderer::SubmitLight(AmbientLight& light)
{
	s_RenderData.DataBuffer.AmbientLight = glm::vec4(light.Color, light.Intensity);
}

void ForwardRenderer::SubmitMesh(Mesh& mesh, glm::mat4 transform, int entity)
{
	s_RenderStats.MeshCount++;
	s_RenderStats.TotalVertices += mesh.GetVertices().size();

	s_RenderData.meshes.push_back(MeshRenderData{ mesh,transform, entity });
}

void ForwardRenderer::SubmitModel(Model& model, glm::mat4 transform, int entity)
{	
	s_RenderData.meshes.reserve(model.GetMeshes().size());

	for (int i = 0; i < model.GetMeshes().size(); i++)
	{
		SubmitMesh(model.GetMeshes()[i], transform, entity);
	}
}

uint32_t ForwardRenderer::GetDepthMapDBG()
{
	return s_RenderData.DepthMapFB->GetDepthAttachmentID();
}

void ForwardRenderer::SubmitLight(PointLight& light)
{
	LightData plData;
	plData.Position = glm::vec4(light.Position, 0.0f);
	plData.Color = glm::vec4(light.Color, light.Intensity);
	plData.Direction.a = light.SpecularPower;
	s_RenderData.LightData.Lights.push_back(plData);

	s_RenderData.DataBuffer.NumLights++;
	s_RenderStats.PointLightCount++;
}

void ForwardRenderer::SubmitLight(DirectionalLight& light)
{
	LightData dlData;
	dlData.Position.w = 1.0f;
	dlData.Direction  = glm::vec4(light.Direction, light.SpecularPower);
	dlData.Color      = glm::vec4(light.Color, light.Intensity);
	s_RenderData.LightData.Lights.push_back(dlData);

	s_RenderData.DataBuffer.NumLights++;
	s_RenderStats.DirectionalLightCount++;
}

ForwardRenderer::RenderStatistics& ForwardRenderer::GetRenderStatistics()
{
	return s_RenderStats;
}

