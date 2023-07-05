#include "EntityListPanel.h"

#include "imgui.h"
#include "LearningEngine.h"


#include "EditorLayer.h"

// GLM
#include "gtc/type_ptr.hpp"
#include <imgui_internal.h>
#include <IconsFontAwesome5.h>
#include <Log/Log.h>

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
		ImGui::Begin("Entities", nullptr, ImGuiWindowFlags_NoCollapse);

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				Entity entity = m_Scene->NewEntity("Entity");
			}
			ImGui::EndPopup();
		}

		auto view = m_Scene->Registry.view<TransformComponent>();
		for (auto entityID : view)
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
				if (ImGui::MenuItem("Duplicate"))
				{
					if (m_SelectedEntity != entt::null)
					{
						Entity entity = m_Scene->NewEntity("Entity");
					}
				}
				if (ImGui::MenuItem("Create Entity"))
				{
					Entity entity = m_Scene->NewEntity("Entity");
				}

				ImGui::EndPopup();
			}

			if (ImGui::IsItemClicked())
			{
				m_SelectedEntity = entityID;
			}

			if (opened)
			{
				
				ImGui::TreePop();
			}
		}

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

	// Adding Components window
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));

		ImGui::Begin("Components", nullptr, ImGuiWindowFlags_NoCollapse);

		// Displaying the components
		if (m_SelectedEntity != entt::null)
		{
			TagComponent& tc = m_Scene->Registry.get<TagComponent>(m_SelectedEntity);
			
			// Rename entity
			char* input = (char*)tc.Tag.c_str();
			auto checked = true;
			if (ImGui::Checkbox("##visible", &checked)) {
				checked = !checked;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetWindowWidth());
			if (ImGui::InputText("##input", input, 30))
			{
				tc.Tag = input;
			}
			ImGui::PopItemWidth();

			if (m_Scene->Registry.try_get<TransformComponent>(m_SelectedEntity))
			{
				TransformComponent& tc = m_Scene->Registry.get<TransformComponent>(m_SelectedEntity);
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::CollapsingHeader("Transform Component"))
				{
					ImGuiContext& g = *GImGui;
					ImGui::BeginColumns("##transform", 2, ImGuiColumnsFlags_NoResize || ImGuiColumnsFlags_NoBorder);
					ImGui::SetColumnWidth(0, 70);
					
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
					ImGui::Text("Position:");
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
					ImGui::Text("Rotation:");
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
					ImGui::Text("Scale:");

					ImGui::NextColumn();
					ImGui::DragFloat3("##position", glm::value_ptr(tc.Position), 0.1f, -1000, 1000, "%.2f");
					ImGui::DragFloat3("##rotation", glm::value_ptr(tc.Rotation), 0.1f, -1000, 1000, "%.2f");
					ImGui::DragFloat3("##scale", glm::value_ptr(tc.Size), 0.1f, -1000, 1000, "%.2f");
					ImGui::EndColumns();
				}
			}

			if (m_Scene->Registry.try_get<QuadRendererComponent>(m_SelectedEntity))
			{
				QuadRendererComponent& qrc = m_Scene->Registry.get<QuadRendererComponent>(m_SelectedEntity);
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::CollapsingHeader("Quad Renderer Component"))
				{
					ImGui::BeginColumns("##quadrenderer", 2, ImGuiColumnsFlags_NoResize ||ImGuiColumnsFlags_NoBorder );
					ImGui::SetColumnWidth(0, 70);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
					ImGui::Text("Scale:");
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
					ImGui::Text("Color:");
					ImGui::NextColumn();
					ImGui::DragFloat3("##scale", glm::value_ptr(qrc.Scale), 0.1f, -1000, 1000, "%.2f");
					ImGui::ColorEdit4("##color", glm::value_ptr(qrc.Color));
					ImGui::EndColumns();
				}
				if (ImGui::Button("Remove Component", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
					m_Scene->Registry.remove<QuadRendererComponent>(m_SelectedEntity);
				}
			}

			if (m_Scene->Registry.try_get<PerspectiveCameraComponent>(m_SelectedEntity))
			{
				PerspectiveCameraComponent& pcc = m_Scene->Registry.get<PerspectiveCameraComponent>(m_SelectedEntity);
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::CollapsingHeader("Perspective Camera Component"))
				{
					ImGui::Checkbox("Main camera", &pcc.MainCamera);

					ImGui::Separator();

					if (ImGui::DragFloat3("Focal point", glm::value_ptr(pcc.FocalPoint)))
						pcc.Camera->SetFocalPoint(pcc.FocalPoint);

					ImGui::Separator();

					if (ImGui::DragFloat("Distance", &pcc.Distance))
						pcc.Camera->SetDistance(pcc.Distance);
					
					if (ImGui::DragFloat("Yaw", &pcc.Yaw))
						pcc.Camera->SetYaw(pcc.Yaw);

					if (ImGui::DragFloat("Pitch", &pcc.Pitch))
						pcc.Camera->SetPitch(pcc.Pitch);

					if (ImGui::DragFloat("FOV", &pcc.FOV))
						pcc.Camera->SetFOV(pcc.FOV);
					
				
					ImGui::Separator();

					ImGui::PushItemWidth(100.0f);
					ImGui::BeginDisabled(pcc.FixedAspectRatio);
					if (ImGui::DragFloat("AspectRatio", &pcc.AspectRatio))
						pcc.Camera->SetAspectRatio(pcc.AspectRatio);
					
					ImGui::PopItemWidth();
					ImGui::EndDisabled();
					
					if (ImGui::Checkbox("Fixed", &pcc.FixedAspectRatio) && pcc.FixedAspectRatio)
					{
						glm::vec2 vpSize = EditorLayer::GetMainViewportSize();

						pcc.AspectRatio = vpSize.x / vpSize.y;
						pcc.Camera->SetAspectRatio(pcc.AspectRatio);
					}


					ImGui::Separator();


					if (ImGui::DragFloat("Near clip", &pcc.NearClip))
						pcc.Camera->SetNearClip(pcc.NearClip);

					if (ImGui::DragFloat("Far clip", &pcc.FarClip))
						pcc.Camera->SetFarClip(pcc.FarClip);

				}
				if (ImGui::Button("Remove Component", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
					m_Scene->Registry.remove<PerspectiveCameraComponent>(m_SelectedEntity);
				}
			}	
			
			ImGui::Separator();

			if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
				ImGui::OpenPopup("ContextMenu");
			}

			if (ImGui::BeginPopup("ContextMenu")) {
				if (ImGui::MenuItem("Quad Renderer Component"))
				{
					QuadRendererComponent qrc;
					m_Scene->Registry.emplace<QuadRendererComponent>(m_SelectedEntity, qrc);
				}
				if (ImGui::MenuItem("Perspective Camera Component"))
				{
					PerspectiveCameraComponent pcc;
					pcc.Camera = new PerspectiveCamera(pcc.FOV, pcc.AspectRatio, pcc.NearClip, pcc.FarClip);
					m_Scene->Registry.emplace<PerspectiveCameraComponent>(m_SelectedEntity, pcc);
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
		ImGui::PopStyleVar(2);
	}

}
