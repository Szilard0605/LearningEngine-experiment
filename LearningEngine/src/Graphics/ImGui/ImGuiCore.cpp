#include "ImGuiCore.h"

//#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui.h>

#include "Platform/ImGui/ImGuiOpenGL.h"
#include "Platform/ImGui/ImGuiGLFW.h"

#include "Core/Application.h"

#include "glfw3.h"

void ImGuiCore::Init(WindowsWindow& window)
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(window.GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450");

}

void ImGuiCore::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiCore::EndFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	Application* app = Application::GetInstance();
	float width = (float)app->GetWindow()->GetWidth();
	float height = (float)app->GetWindow()->GetHeight();
	io.DisplaySize = ImVec2(width, height);
	

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiCore::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}