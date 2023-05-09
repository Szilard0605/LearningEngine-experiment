#include "EditorLayer.h"

#include "imgui.h"

EditorLayer::EditorLayer() : Layer("LearningEngine Editor")
{
	printf("Editor started\n");
}

void EditorLayer::OnAttach()
{
	printf("Editor attached\n");

	//GLRenderer::ClearColor(1, 0, 1, 1);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnEvent(Event& event)
{
}

void EditorLayer::OnImGuiRender()
{
	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PopStyleVar();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PopStyleVar();
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	/*if (opt_fullscreen)
		ImGui::PopStyleVar(2);*/

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}



	ImGui::Begin("Viewport");
	ImVec2 crAvail = ImGui::GetContentRegionAvail();
	printf("Viewport size: %f, %f\n", crAvail.x, crAvail.y);

	ImGui::End();

	ImGui::Begin("Entities");
	for (int i = 0; i < 10; i++)
	{
		std::string label = std::string("Entity ") + std::to_string(i);
 		ImGui::Selectable(label.c_str(), false);
	}
	ImGui::End();


	ImGui::Begin("Log");

	ImGui::End();

	ImGui::End();

}

bool EditorLayer::OnKeyChange(KeyEvent& keyevent)
{
	return false;
}

bool EditorLayer::OnMouseButtonChange(MouseButtonEvent& event)
{
	return false;
}

bool EditorLayer::OnMouseMove(MouseMoveEvent& event)
{
	return false;
}

bool EditorLayer::OnMouseScrolled(MouseScrolledEvent& event)
{
	return false;
}

void EditorLayer::OnUpdate(Timestep timestep)
{
	Renderer2D::ClearColor(glm::vec4(1, 1, 1, 1));
}
