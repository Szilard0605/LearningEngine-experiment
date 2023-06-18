#include "PhysicsTest.h"
#include "glm.hpp"

#include "imgui.h"

static glm::vec3 debugPos = { 0, 0, -5000 };
static glm::vec3 debugScale = {1, 1, 1};
static glm::vec3 debugRot = { 0, 0, 0 };

static glm::vec3 debugPos_Pants = { 0, 0, -3500 };
static glm::vec3 debugScale_Pants = { 1, 1, 1 };
static glm::vec3 debugRot_Pants = { 0, 0, 0 };

static Mesh* mesh;

static bool enable = false;
static glm::vec4 MeshColor = {1.0f, 1.0f, 1.0f, 1.0f};
static glm::vec4 clearcolor = {0.1f, 0.1f, 0.1f, 1.0f};

Texture2D* testTexture;
Texture2D* testTexture2;

glm::vec2 LastMousePos = {-1, -1};

glm::vec3 CubePos = {0, 0, 0};

glm::vec4 u_Color;
glm::vec3 u_LightPosition;
float u_LightAttenuation;
glm::vec4 u_LightColor = { 1, 1, 1, 1};;
glm::vec3 u_LightDir = {1, 1, 1};

PhysicsTest::PhysicsTest() : Layer("PhysicsTest")
{
	//GLRenderer::EnableBlend(true); 
	//GLRenderer::EnableDepthBuffer(true);
}

void PhysicsTest::OnAttach()
{
	Renderer2D::ClearColor({0.1f, 0.1f, 0.1f, 1.0f});

	Camera = PerspectiveCamera(60.0f, 1.7777f, 0.1f, 10000.0f);

	testShader = Shader::Create("res/shaders/default_shader.shader");
	
	testTexture = Texture2D::Create(1, 1);
	uint32_t tdata = 0xffffffff;
	testTexture->SetData(&tdata, sizeof(uint32_t));

	testTexture2 = Texture2D::Create("res/models/troll/textures/TEX_pants_baseColor.png");
	//TEX_pants_baseColor.png

	testMaterial = Material(testShader);

	testModel = Model("res/models/sponza/Sponza.gltf", testMaterial);

	testMaterial2 = Material(testShader);
	testMaterial2.SetTexture(testTexture);
	
	mesh = MeshFactory::CreateCube(20000000*100, testMaterial2);


}

void PhysicsTest::OnDetach()
{
	//delete texture;
}

void PhysicsTest::OnUpdate(Timestep timestep)
{
	Renderer2D::ClearColor(clearcolor);
	Renderer2D::Clear();

	testModel.Render(Camera, debugPos, { 1, 1, 1 }, {0, 0, 0});
	mesh->Render(Camera, CubePos, debugScale, debugRot);

	float speed = 0.001f;

	u_LightPosition = CubePos;

	if (Input::IsKeyPressed(Key::LeftShift))
		speed *= 2;

	if (Input::IsKeyPressed(Key::W))
	{
		Camera.Translate(Camera.GetPosition() + (speed * Camera.GetForwardDirection()));
	}

	if (Input::IsKeyPressed(Key::S))
	{
		Camera.Translate(Camera.GetPosition() - (speed * Camera.GetForwardDirection()));
	}

	if (Input::IsKeyPressed(Key::D))
	{
		Camera.Translate(Camera.GetPosition() + (speed * Camera.GetRightDirection()));
	}
	if (Input::IsKeyPressed(Key::A))
	{
		Camera.Translate(Camera.GetPosition() - (speed * Camera.GetRightDirection()));
	}

	
	//testMaterial2.Bind();

	testMaterial.GetShader()->SetVec3f("u_LightPosition", CubePos);
	//testMaterial2.GetShader().SetVec3f("u_LightPosition", CubePos);
	testMaterial.GetShader()->SetFloat("u_LightAttenuation", u_LightAttenuation);
	//testMaterial2.GetShader().SetFloat("u_LightAttenuation", u_LightAttenuation);
	testMaterial.GetShader()->SetVec4f("u_LightColor", u_LightColor);
	//testMaterial2.GetShader().SetVec4f("u_LightColor", u_LightColor);
	testMaterial.GetShader()->SetVec3f("u_LightDir", Camera.GetForwardDirection());
	//testMaterial2.GetShader().SetVec3f("u_LightDir", -Camera.GetForwardDirection());

	testMaterial.GetShader()->SetVec4f("u_Color", { 1, 1, 1, 1  });
	//testMaterial2.GetShader().SetVec4f("u_Color", { 1, 1, 1, 1 });
	
	testMaterial.Bind();
}

void PhysicsTest::OnImGuiRender()
{
	ImGui::Begin("Settings");
	{
		{
			float pos[3] = { debugPos.x, debugPos.y, debugPos.z };
			ImGui::DragFloat3("SkullPos", pos);
			debugPos = { pos[0], pos[1], pos[2] };

			float scale[3] = { debugScale.x, debugScale.y, debugScale.z };
			ImGui::DragFloat3("SkullScale", scale);
			debugScale = { scale[0], scale[1], scale[2] };

			float rot[3] = { debugRot.x, debugRot.y, debugRot.z };
			ImGui::DragFloat3("SkullRot", rot);
			debugRot = { rot[0], rot[1], rot[2] };
		}
		ImGui::NewLine();
		{
			float pos[3] = { debugPos_Pants.x, debugPos_Pants.y, debugPos_Pants.z };
			ImGui::DragFloat3("Pants Pos", pos);
			debugPos_Pants = { pos[0], pos[1], pos[2] };

			float scale[3] = { debugScale_Pants.x, debugScale_Pants.y, debugScale_Pants.z };
			ImGui::DragFloat3("Pants Scale", scale, 0.5f);
			debugScale_Pants = { scale[0], scale[1], scale[2] };

			float rot[3] = { debugRot_Pants.x, debugRot_Pants.y, debugRot_Pants.z };
			ImGui::DragFloat3("Pants Rot", rot);
			debugRot_Pants = { rot[0], rot[1], rot[2] };

		}
		ImGui::NewLine();
		{
			ImGui::DragFloat("Spotlight Strength", &u_LightAttenuation, 0.1f);

			float pos[3] = { u_LightPosition.x, u_LightPosition.y, u_LightPosition.z };
			ImGui::DragFloat3("Spotlight pos", pos);
			u_LightPosition = { pos[0], pos[1], pos[2] };

			float color[4] = { u_LightColor.x, u_LightColor.y, u_LightColor.z, u_LightColor.a };
			ImGui::DragFloat4("Spotlight color", color, 0.05f);
			u_LightColor = { color[0], color[1], color[2], color[3] };


			float dir[3] = { u_LightDir.x, u_LightDir.y, u_LightDir.z };
			ImGui::DragFloat4("Spotlight dir", dir, 0.05f);
			u_LightDir = { dir[0], dir[1], dir[2] };
		}
		ImGui::NewLine();
		{
			float ccolor[4] = { clearcolor.x, clearcolor.y, clearcolor.z, clearcolor.a };
			ImGui::DragFloat4("Clear color", ccolor, 0.05f);
			clearcolor = { ccolor[0], ccolor[1], ccolor[2], ccolor[3] };
		}
	}
	ImGui::End();
}

bool PhysicsTest::OnKeyChange(KeyEvent& keyEvent)
{

	return true;
}

bool PhysicsTest::OnMouseButtonChange(MouseButtonEvent& event)
{
	if (event.GetButton() == MouseButton::RIGHT_CLICK && event.GetAction() == KeyAction::RELEASED)
	{
		LastMousePos = {-1, -1};
	}
	return true;
}

bool PhysicsTest::OnMouseMove(MouseMoveEvent& event)
{
	if (Input::IsMouseButtonPressed(MouseButton::RIGHT_CLICK))
	{
		if(LastMousePos == glm::vec2(-1, -1))
			LastMousePos = event.GetPosition();

		Camera.SetYaw(Camera.GetYaw() + (event.GetPosition().x - LastMousePos.x) * 0.01f);
		Camera.SetPitch(Camera.GetPitch() + (event.GetPosition().y - LastMousePos.y) * 0.01f);

		LastMousePos = event.GetPosition();
	}
	return true;
}

bool PhysicsTest::OnMouseScrolled(MouseScrolledEvent& event)
{

	return false;
}

void PhysicsTest::OnEvent(Event& event)
{
	event.Dispatch<KeyEvent>(BIND_EVENT_FN(PhysicsTest::OnKeyChange));
	event.Dispatch<MouseMoveEvent>(BIND_EVENT_FN(PhysicsTest::OnMouseMove));
	event.Dispatch<MouseButtonEvent>(BIND_EVENT_FN(PhysicsTest::OnMouseButtonChange));
	event.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(PhysicsTest::OnMouseScrolled));
}
