#include "DemoLayer.h"
#include "glm.hpp"

#include "imgui.h"

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

	demoMaterial = Material(Shader::Create("res/shaders/default_shader.shader"));
	demoModel = Model("res/models/Sponza/sponza.obj", demoMaterial);
}

void DemoLayer::OnDetach()
{
	//delete texture;
}

void DemoLayer::OnUpdate(Timestep timestep)
{
	Renderer2D::ClearColor(clearColor);
	Renderer2D::Clear();

	demoMaterial.GetShader()->SetFloat("u_AmbientStrength", 0.03f);
	demoMaterial.GetShader()->SetFloat("u_DiffuseStrength", 1.0f);
	demoMaterial.GetShader()->SetFloat("u_SpecularStrength", 0.5f);
	demoMaterial.GetShader()->SetVec4f("u_AmbientColor", {1, 1, 1, 1});
	demoMaterial.GetShader()->SetVec4f("u_DiffuseColor", { 1, 1, 1, 1 });
	demoMaterial.GetShader()->SetVec3f("u_ViewPos", Camera.GetPosition());
	demoMaterial.GetShader()->SetVec3f("u_DiffusePosition", {0, 50, 0});
	demoMaterial.GetShader()->SetBool("u_UseNormalMap", true);

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
