#include "ContentBrowser.h"

#include "imgui.h"

#include "LearningEngine.h"

ContentBrowser::ContentBrowser(Scene* scene)
	: m_Scene(scene)
{
}

void ContentBrowser::Render()
{
	if (!m_Scene)
		return;

	ImGui::Begin("Content browser");
	//ImGui::Text("I am a log string");
	ImGui::End();


}
