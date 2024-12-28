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

	CubeMap* SkyboxCubeMap;
	Shader* SkyboxShader;
	VertexArray* SkyboxVA;
	IndexBuffer* SkyboxIB;
	VertexBuffer* SkyboxVB;
	std::vector<float> SkyboxVertices;
	std::vector<uint32_t> SkyboxIndices;
};

static RenderData s_RenderData;
static ForwardRenderer::RenderStatistics s_RenderStats;

void ForwardRenderer::Init(RendererAPI* rendererapi)
{
	s_RenderData.shader = ShaderLibrary::Load("ForwardRenderer", "res/shaders/default_shader.shader");


	s_RenderData.LightBuffer = ShaderBuffer::Create(sizeof(LightData) * MAX_LIGHTS);
	s_RenderData.LightData.Lights.reserve(MAX_LIGHTS);

	
	s_RenderData.RenderDataBuffer = ShaderBuffer::Create(sizeof(RenderDataSB));

	// Directional shadow mapping setup
	FramebufferSpecifications depthFBSpecs;
	depthFBSpecs.Attachments = { FramebufferAttachment::Depth };
	depthFBSpecs.Width = 2048;
	depthFBSpecs.Height = 2048;
	s_RenderData.DepthMapFB = Framebuffer::Create(depthFBSpecs);

	s_RenderData.DepthMapShader = Shader::Create("res/shaders/depthmap.shader");

	// Skybox setup
	s_RenderData.SkyboxShader = Shader::Create("res/shaders/skybox.shader");
	s_RenderData.SkyboxVA = VertexArray::Create();
	s_RenderData.SkyboxVertices = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f
	};

	s_RenderData.SkyboxIndices = {
		0, 1, 2, 2, 3, 0, // Back face
		4, 5, 1, 1, 0, 4, // Left face
		3, 2, 6, 6, 7, 3, // Right face
		7, 6, 5, 5, 4, 7, // Front face
		4, 0, 3, 3, 7, 4, // Top face
		1, 5, 6, 6, 2, 1  // Bottom face
	};
	s_RenderData.SkyboxVB = VertexBuffer::Create(sizeof(float) * s_RenderData.SkyboxVertices.size());
	s_RenderData.SkyboxVA = VertexArray::Create();
	s_RenderData.SkyboxVB->SetLayout({ 
		{ ShaderDataType::Float3, "a_position" }
	});
	s_RenderData.SkyboxVA->AddVertexBuffer(s_RenderData.SkyboxVB);
	s_RenderData.SkyboxIB = IndexBuffer::Create(s_RenderData.SkyboxIndices.data(), static_cast<uint32_t>(s_RenderData.SkyboxIndices.size()));
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

// Set the orthographic projection matrix
static float s_nearPlane = 0.1f;
static float s_farPlane = 1000.0f;
static float s_left = -10.0f; // Adjust based on your scene
static float s_right = 10.0f;
static float s_bottom = -10.0f;
static float s_top = 10.0f;
static float s_lightDist = 64.0f;
static float s_sceneSize = 10.0f;

void RenderSkybox()
{
	if (!s_RenderData.SkyboxCubeMap)
		return;

	s_RenderData.SkyboxShader->Bind();

	s_RenderData.SkyboxShader->SetMatrix4f("u_View", glm::mat4(glm::mat3(s_RenderData.camera.GetViewMatrix())));
	s_RenderData.SkyboxShader->SetMatrix4f("u_Projection", s_RenderData.camera.GetProjectionMatrix());

	s_RenderData.SkyboxShader->SetInt("u_Skybox", 0);
	s_RenderData.SkyboxCubeMap->Bind(0);

	s_RenderData.SkyboxVB->SetData(s_RenderData.SkyboxVertices.data(), sizeof(float) * s_RenderData.SkyboxVertices.size());
	s_RenderData.SkyboxVA->Bind();
	s_RenderData.SkyboxIB->Bind();

	s_RenderData.SkyboxVA->DrawIndexed(s_RenderData.SkyboxIB->GetCount());
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
			glm::vec3 lightPosition = -lightDirection * s_lightDist; // Place light far away from origin
			glm::vec3 lightTarget = lightPosition + lightDirection; // The target is the origin or the center of the scene
			//glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector, usually the y-axis
			glm::vec3 upVector = abs(lightDirection.y) > 0.9f ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
			glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), upVector);
			

			glm::mat4 lightProjection = glm::ortho(-s_sceneSize, s_sceneSize, -s_sceneSize, s_sceneSize, s_nearPlane, s_farPlane);
			lightSpaceMatrix = lightProjection * lightView;

			// Send lightSpaceMatrix to the shader
			s_RenderData.DepthMapShader->Bind();

			RendererAPI* api = Application::GetInstance()->GetRenderer();
			api->SetViewportSize(2048, 2048);
			s_RenderData.DepthMapFB->Bind();
			api->ClearDepthBuffer();
			for (int i = 0; i < s_RenderData.meshes.size(); i++)
			{

				s_RenderData.DepthMapShader->SetMatrix4f("u_LightSpaceMatrix", lightSpaceMatrix * s_RenderData.meshes[i].transform);
				//s_RenderData.DepthMapShader->SetMatrix4f("u_Model", s_RenderData.meshes[i].transform);
				s_RenderStats.DrawCalls++;
				s_RenderData.meshes[i].mesh.Render(s_RenderData.camera, s_RenderData.meshes[i].transform, s_RenderData.meshes[i].EntityID);
				
			}
		}
	}
	
	FrameBuffer->Bind();
	api->Clear({ 0.5, 0.5, 0.5, 1 });
	api->ClearDepthBuffer();
	
	api->SetDepthFunc(RendererAPI::DepthFunc::LEQUAL);
	RenderSkybox();
	api->SetDepthFunc(RendererAPI::DepthFunc::LESS);
	s_RenderData.RenderDataBuffer->Bind(0);
	s_RenderData.RenderDataBuffer->SetData(&s_RenderData.DataBuffer, sizeof(RenderDataSB), 0);

	// Light setup
	s_RenderData.LightBuffer->Bind(1);
	s_RenderData.LightBuffer->SetData(s_RenderData.LightData.Lights.data(), sizeof(LightData) * MAX_LIGHTS, 0);

	s_RenderData.shader->Bind();
	s_RenderData.DepthMapFB->BindDepthTexture(1);
	s_RenderData.shader->SetInt("u_DepthMap", 1);

	for (int i = 0; i < s_RenderData.meshes.size(); i++)
	{
		s_RenderStats.DrawCalls++;
		s_RenderData.meshes[i].mesh.GetMaterial()->GetShader()->SetMatrix4f("u_ViewProjection", s_RenderData.camera.GetViewProjection());
		s_RenderData.meshes[i].mesh.GetMaterial()->GetShader()->SetMatrix4f("u_Transform", s_RenderData.meshes[i].transform);
		s_RenderData.meshes[i].mesh.GetMaterial()->GetShader()->SetMatrix4f("u_LightSpaceMatrix", lightSpaceMatrix * s_RenderData.meshes[i].transform);
		
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

void ForwardRenderer::SetSkybox(CubeMap* cubeMap)
{
	s_RenderData.SkyboxCubeMap = cubeMap;
}

void ForwardRenderer::DBGOrtho(float nearPlane, float farPlane, float left, float right, float bottom, float top, float lightDist, float sceneSize)
{
	s_nearPlane = nearPlane;
	s_farPlane = farPlane;
	s_left = left;
	s_right = right;
	s_bottom = bottom;
	s_top = top;
	s_lightDist = lightDist;
	s_sceneSize = sceneSize;
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
	dlData.Color = glm::vec4(light.Color, light.Intensity);
	s_RenderData.LightData.Lights.push_back(dlData);

	s_RenderData.DataBuffer.NumLights++;
	s_RenderStats.DirectionalLightCount++;
}

ForwardRenderer::RenderStatistics& ForwardRenderer::GetRenderStatistics()
{
	return s_RenderStats;
}

