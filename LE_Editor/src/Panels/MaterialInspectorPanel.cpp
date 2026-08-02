#include "MaterialInspectorPanel.h"

#include "imgui.h"

#include "entt.hpp"
#include <gtc/type_ptr.hpp>
#include <imgui_internal.h>

MaterialInspectorPanel::MaterialInspectorPanel(EntityListPanel* entitylistpanel)
	: m_elpRef(entitylistpanel)
{
}

void MaterialInspectorPanel::Render()
{
	ImGui::Begin("Material Inspector");

	Entity entity(m_elpRef->GetSelectedEntity(), m_elpRef->GetScene());

	if (!entity.IsValid() || !m_elpRef->GetScene() || !entity.HasComponent<StaticModelComponent>())
	{
		ImGui::End();
		return;
	}

	/*auto& smc = entity.GetComponent<StaticModelComponent>();
	if (smc.StaticModel)
	{
		// TODO: Make all of the meshes show in EntityListPanel under every model, 
		// so we can select each mesh separately, and we can save CPU cycles

		auto& Meshes = smc.StaticModel->GetMeshes();
		for (int i = 0; i < Meshes.size(); i++)
		{
			if (ImGui::CollapsingHeader(smc.StaticModel->GetMeshes()[i].GetMaterial()->GetName().c_str()))
			{
				ImGui::PushID(Meshes[i].GetMaterial()->GetName().c_str());
				ImGui::ColorEdit4("BaseColor", glm::value_ptr(Meshes[i].GetMaterial()->BaseColor));
				ImGui::PopID();
			}
		}
	}*/
	ImGui::End();
}
