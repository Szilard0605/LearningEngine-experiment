#include "SceneRendererPanel.h"

#include "imgui.h"
#include <gtc/type_ptr.hpp>

SceneRendererPanel::SceneRendererPanel(Scene* scene)
	: m_Scene(scene)
{
}

SceneRendererPanel::~SceneRendererPanel()
{
}

void SceneRendererPanel::Render()
{
	ImGui::Begin("Scene Renderer");

	if (ImGui::CollapsingHeader("Skybox"))
	{
		if (ImGui::Button("Load"))
		{
			std::string CubeMapPath;
			if (Utils::FileDialog::OpenFile("Cube map (*.all)\0*.**\0", CubeMapPath))
			{
				//m_Scene->SetSkyboxCubeMap(CubeMapPath, CubeMapLayout::HorizontalCross);
			}
		}
	}

	if (ImGui::CollapsingHeader("Ambient Light"))
	{
		AmbientLight ambient = m_Scene->GetAmbientLight();

		ImGui::DragFloat("Intensity", &ambient.Intensity);
		ImGui::ColorEdit3("Color", glm::value_ptr(ambient.Color));

		m_Scene->SetAmbientLight(ambient);
	}

	ImGui::End();
}
