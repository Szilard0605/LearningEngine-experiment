#include "EntityListPanel.h"

#include "imgui.h"

#include "LearningEngine.h"

// GLM
#include "gtc/type_ptr.hpp"

EntityListPanel::EntityListPanel(Scene* scene)
	: m_Scene(scene)
{
}

void EntityListPanel::Render()
{
	if (!m_Scene)
		return;

	//Entity window
	{
		ImGui::Begin("Entities");

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				Entity entity = m_Scene->NewEntity("Entity");
			}
			ImGui::EndPopup();
		}

		m_Scene->Registry.each([this](auto entityID)
		{
			ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entityID) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

			TagComponent& tc = m_Scene->Registry.get<TagComponent>(entityID);
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entityID, flags, tc.Tag.c_str());

			if (ImGui::BeginPopupContextItem())
			{
				m_SelectedEntity = entityID;

				if (ImGui::MenuItem("Delete"))
				{
					if (m_SelectedEntity != entt::null)
					{
						m_DeleteEntity = true;
					}
				}

				// Rename entity
				ImGui::PushItemWidth(100.0f);
				char* input = (char*)tc.Tag.c_str();
				if (ImGui::InputTextWithHint("##name", "Rename", input, 30))
				{
					tc.Tag = input;
				}
				ImGui::PopItemWidth();

				ImGui::EndPopup();
			}

			if (ImGui::IsItemClicked())
			{
				m_SelectedEntity = entityID;
				printf("Selected Entity %d\n", entityID);
			}

			if (opened)
			{

				ImGui::TreePop();
			}
		});

		if (ImGui::IsWindowFocused())
		{
			if (m_DeleteEntity || Input::IsKeyPressed(Key::Delete) && m_SelectedEntity != entt::null)
			{
				ImGui::OpenPopup("Delete entity?");
			}
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Delete entity?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			center = ImGui::GetWindowViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			TagComponent& tc = m_Scene->Registry.get<TagComponent>(m_SelectedEntity);
			ImGui::Text((std::string("Are you sure you want to delete this entity? (") + tc.Tag + ")").c_str());
		
			ImVec2 crAvail = ImGui::GetContentRegionAvail();
			ImGui::SetCursorPosX(((crAvail.x - ImGui::CalcTextSize("Delete").x) * 0.5f) - ImGui::CalcTextSize("Delete").x);
			if (ImGui::Button("Delete") || Input::IsKeyPressed(Key::Enter))
			{
				if (m_SelectedEntity != entt::null)
				{
					m_Scene->Registry.destroy(m_SelectedEntity);
					m_SelectedEntity = entt::null;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::SameLine();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5.0f);

			if (ImGui::Button("Cancel") || Input::IsKeyPressed(Key::Escape))
			{
				ImGui::CloseCurrentPopup();
				
			}

			m_DeleteEntity = false;

			ImGui::EndPopup();
		}

		ImGui::End();
	}
	// Components window
	{

		ImGui::Begin("Components");

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::BeginMenu("Add Component"))
			{
				if (ImGui::MenuItem("Quad Renderer Component"))
				{
					QuadRendererComponent qrc;
					m_Scene->Registry.emplace<QuadRendererComponent>(m_SelectedEntity, qrc);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (m_SelectedEntity != entt::null && m_Scene->Registry.has<TransformComponent>(m_SelectedEntity))
		{
			TransformComponent& tc = m_Scene->Registry.get<TransformComponent>(m_SelectedEntity);
			if (ImGui::CollapsingHeader("Transform Component"))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(tc.Position), 0.1f);
				ImGui::DragFloat3("Size", glm::value_ptr(tc.Size), 0.1f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(tc.Rotation), 0.1f);
			}
		}

		if (m_SelectedEntity != entt::null && m_Scene->Registry.has<QuadRendererComponent>(m_SelectedEntity))
		{
			QuadRendererComponent& qrc = m_Scene->Registry.get<QuadRendererComponent>(m_SelectedEntity);
			if (ImGui::CollapsingHeader("Quad Renderer Component"))
			{
				ImGui::DragFloat3("Scale", glm::value_ptr(qrc.Scale), 0.1f);
				ImGui::ColorEdit4("Color", glm::value_ptr(qrc.Color));
			}
		}

		ImGui::End();
	}

}
