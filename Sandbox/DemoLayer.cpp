#include "DemoLayer.h"
#include "glm.hpp"

#include "imgui.h"
#include <gtc/type_ptr.hpp>

static glm::vec3 sp_pos = {0, 0, 0};


static glm::vec3 ambdir = { -0.2f, -1.0f, -0.3f };
static glm::vec3 ambient = { 1.0f, 0.0f, 0.0f };
static glm::vec3 ambdiffuse = { 0.4f, 0.4f, 0.4f };
static glm::vec3 ambspecular = { 0.5f, 0.5f, 0.5f };

static float s_Timestep = 0.0f;

DemoLayer::DemoLayer() : 
	Layer("DemoLayer")
{
	//GLRenderer::EnableBlend(true); 
	//GLRenderer::EnableDepthBuffer(true);
}

void DemoLayer::OnAttach()
{
	Renderer2D::ClearColor({0.1f, 0.1f, 0.1f, 1.0f});

	Camera = PerspectiveCamera(60.0f, 1.7777f, 0.1f, 10000.0f);

	demoMaterial = Material(ShaderLibrary::GetShader("ForwardRenderer"));
	demoModel = Model("res/models/Sponza/sponza.obj", demoMaterial);
}

void DemoLayer::OnDetach()
{
	//delete texture;
}

void DemoLayer::OnUpdate(Timestep timestep)
{
	s_Timestep = timestep.Milliseconds();

	Renderer2D::ClearColor(clearColor);
	Renderer2D::Clear();


	//demoModel.Render(Camera, glm::mat4(1.0f));
	ForwardRenderer::BeginScene(Camera);

	ForwardRenderer::SubmitModel(&demoModel, glm::mat4(1.0f));
	ForwardRenderer::EndScene();

	ForwardRenderer::Present();

	float speed = 3.0f;

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
}

void DemoLayer::OnImGuiRender()
{
	ImGui::Begin("Render Statistics");

	ImGui::Text("Draw calls: %d", ForwardRenderer::GetRenderStatistics().DrawCalls);
	ImGui::Text("Frametime: %f (%d FPS)", s_Timestep, (int)glm::floor((1.0f / s_Timestep) * 1000.0f));
	ImGui::Text("Mesh count: %d (%d vertices)", ForwardRenderer::GetRenderStatistics().MeshCount,
												ForwardRenderer::GetRenderStatistics().Vertices);

	ImGui::End();
}

bool DemoLayer::OnKeyChange(KeyEvent& keyEvent)
{

	return true;
}

bool DemoLayer::OnMouseButtonChange(MouseButtonEvent& event)
{
	if (event.GetButton() == MouseButton::RIGHT_CLICK && event.GetAction() == KeyAction::RELEASED)
	{
		LastMousePos = {-1, -1};
	}
	return true;
}

bool DemoLayer::OnMouseMove(MouseMoveEvent& event)
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

bool DemoLayer::OnMouseScrolled(MouseScrolledEvent& event)
{

	return false;
}

void DemoLayer::OnEvent(Event& event)
{
	event.Dispatch<KeyEvent>(BIND_EVENT_FN(DemoLayer::OnKeyChange));
	event.Dispatch<MouseMoveEvent>(BIND_EVENT_FN(DemoLayer::OnMouseMove));
	event.Dispatch<MouseButtonEvent>(BIND_EVENT_FN(DemoLayer::OnMouseButtonChange));
	event.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(DemoLayer::OnMouseScrolled));
}
