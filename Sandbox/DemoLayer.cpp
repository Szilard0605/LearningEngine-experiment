#include "DemoLayer.h"
#include "glm.hpp"

#include "imgui.h"

DemoLayer::DemoLayer() : Layer("DemoLayer")
{
	//GLRenderer::EnableBlend(true); 
	//GLRenderer::EnableDepthBuffer(true);
}

void DemoLayer::OnAttach()
{
	Renderer2D::ClearColor({0.1f, 0.1f, 0.1f, 1.0f});

	Camera = PerspectiveCamera(60.0f, 1.7777f, 0.1f, 10000.0f);

	cubeTransform = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -2.0f});

	Shader* demoShader = Shader::Create("res/shaders/texture_shader.shader");
	
	demoTexture = Texture2D::Create(1, 1);
	uint32_t tdata = 0xffffffff;
	demoTexture->SetData(&tdata, sizeof(uint32_t));

	demoMaterial = new Material(demoShader);
	demoMaterial->SetTexture(demoTexture);
	
	demoMesh = MeshFactory::CreateCube(1, *demoMaterial);
	demoMesh->SetMaterial(demoMaterial);
}

void DemoLayer::OnDetach()
{
	//delete texture;
}

void DemoLayer::OnUpdate(Timestep timestep)
{
	Renderer2D::ClearColor(clearColor);
	Renderer2D::Clear();

	demoMesh->Render(Camera, cubeTransform);

	float speed = 0.001f;

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
