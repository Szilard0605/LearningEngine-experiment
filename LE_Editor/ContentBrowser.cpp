#include "ContentBrowser.h"

#include "imgui.h"

#include "LearningEngine.h"

Image image;
static Texture2D* texture;

ContentBrowser::ContentBrowser(Scene* scene)
	: m_Scene(scene)
{
	
	image = Image("res/textures/ball.png");
	//ImageAsset testAsset(image);
	texture = Texture2D::Create(image);

}

void ContentBrowser::Render()
{
	if (!m_Scene)
		return;

	ImGui::Begin("Content browser");
	ImGui::Image((ImTextureID)texture->GetTextureID(), ImVec2(image.GetProperties().Width, image.GetProperties().Height));
	ImGui::End();


}
