#include "LauncherLayer.h"

LauncherLayer::LauncherLayer() 
	: Layer("Launcher Layer")
{

}

void LauncherLayer::OnAttach()
{
}

void LauncherLayer::OnDetach()
{
}

void LauncherLayer::OnEvent(Event& event)
{
}

void LauncherLayer::OnImGuiRender()
{
}


void LauncherLayer::OnUpdate(Timestep timestep)
{
	Renderer2D::ClearColor({ 1, 1, 1, 1 });
}

bool LauncherLayer::OnKeyChange(KeyEvent& keyevent)
{
	return false;
}

bool LauncherLayer::OnMouseButtonChange(MouseButtonEvent& event)
{
	return false;
}

bool LauncherLayer::OnMouseMove(MouseMoveEvent& event)
{
	return false;
}

bool LauncherLayer::OnMouseScrolled(MouseScrolledEvent& event)
{
	return false;
}

