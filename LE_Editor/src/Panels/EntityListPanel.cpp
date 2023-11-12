#include "EntityListPanel.h"

#include "LearningEngine.h"

#include "imgui.h"
#include <imgui_internal.h>

#include "../EditorLayer.h"
// GLM
#include "gtc/type_ptr.hpp"

#include <IconsFontAwesome5.h>

EntityListPanel::EntityListPanel(Scene *scene)
	: m_Scene(scene)
{
}

void EntityListPanel::DisplayHierarchy(Entity entity)
{
	ImGui::PushID(reinterpret_cast<const void*>(static_cast<intptr_t>(entity.GetHandle())));

	HierarchyComponent& hc = entity.GetComponent<HierarchyComponent>();

	bool hasChild = hc.Children.size() > 0;
	bool hasParent = hc.Parent != entt::null;

	TagComponent& tc = entity.GetComponent<TagComponent>();
	auto entityStr = tc.Tag.c_str();

	ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity.GetHandle()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	if (!hasChild)
		flags |= ImGuiTreeNodeFlags_Leaf;


	bool nodeOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetHandle(), flags, entityStr);

	if (ImGui::IsItemClicked())
		m_SelectedEntity = entity.GetHandle();

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("Entity", &entity, sizeof(Entity));
		ImGui::Text(entityStr);
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
		{ 
			Entity payload_n = *static_cast<const Entity*>(payload->Data);
			HierarchyComponent& payloadHC = payload_n.GetComponent<HierarchyComponent>();
			
			if (payload_n.GetParent() != entity)
			{
				Entity& oldParent = payload_n.GetParent();
				if (oldParent.IsValid())
				{
					HierarchyComponent& oldParentHC = oldParent.GetComponent<HierarchyComponent>();
					oldParentHC.Children.erase(std::remove(oldParentHC.Children.begin(), oldParentHC.Children.end(), payload_n.GetHandle()), oldParentHC.Children.end());
				}
				
				if (!payload_n.GetParent().IsValid())
				{
					payload_n.SetParent(entity);
				}
				else
				{
					payloadHC.Parent = entt::null;
					payload_n.SetParent(entity);
				}
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (hasChild && nodeOpen)
	{
		for (int i = 0; i < hc.Children.size(); i++)
		{
			Entity child(hc.Children[i], m_Scene);
			if(child.IsValid())
				DisplayHierarchy(child);
		}
	}

	if(nodeOpen)
		ImGui::TreePop();

	ImGui::PopID();
}


void EntityListPanel::SetScene(Scene* scene)
{
	m_Scene = scene;
	m_SelectedEntity = entt::null;
}

void EntityListPanel::Render()
{
	if (!m_Scene)
	{
		LE_CLIENT_ERROR("There is no scene!");
		return;
	}
	// Entity window
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

		if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
				{
					Entity payload_n = *static_cast<const Entity*>(payload->Data);
					HierarchyComponent& payloadHC = payload_n.GetComponent<HierarchyComponent>();
					Entity& oldParent = payload_n.GetParent();
					if (oldParent.IsValid())
					{
						HierarchyComponent& oldParentHC = oldParent.GetComponent<HierarchyComponent>();
						oldParentHC.Children.erase(std::remove(oldParentHC.Children.begin(), oldParentHC.Children.end(), payload_n.GetHandle()), oldParentHC.Children.end());
						payloadHC.Parent = entt::null;
					}
				}
			}
			
			// Render Entity hierarchy
			auto view = m_Scene->Registry.view<TransformComponent>();

			for (auto it = view.begin(); it < view.end(); it++)
			{
				Entity h_entity = Entity(view[it.index()], m_Scene);
				HierarchyComponent& hc = h_entity.GetComponent<HierarchyComponent>();

				if (hc.Parent == entt::null)
					DisplayHierarchy(h_entity);
			}
			ImGui::TreePop();
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
					m_Scene->DestroyEntity(Entity(m_SelectedEntity, m_Scene));
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
				if (ImGui::CollapsingHeader("Transform"))
				{
					ImGui::BeginColumns("##transform", 2, ImGuiColumnsFlags_NoResize || ImGuiColumnsFlags_NoBorder);
					ImGui::SetColumnWidth(0, 70);

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
					ImGui::Text("Position:");
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
					ImGui::Text("Rotation:");
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
					ImGui::Text("Scale:");

					ImGui::NextColumn();

					glm::vec3 deltaPosition = tc.Position;
					ImGui::DragFloat3("##position", glm::value_ptr(tc.Position), 0.1f, -1000, 1000, "%.2f");
					deltaPosition -= tc.Position;
					

					glm::vec3& degRot = glm::degrees(tc.Rotation);
					glm::vec3 deltaRotation = tc.Rotation;
					ImGui::DragFloat3("##rotation", glm::value_ptr(degRot), 0.1f, -1000, 1000, "%.2f");
					tc.Rotation = glm::radians(degRot);
					deltaRotation -= tc.Rotation;


					glm::vec3 deltaScale = tc.Scale;
					ImGui::DragFloat3("##scale", glm::value_ptr(tc.Scale), 0.1f, -1000, 1000, "%.2f");
					deltaScale -= tc.Scale;
					
					Entity s_entity = Entity(m_SelectedEntity, m_Scene);
					if (s_entity.GetChildren().size())
					{
						for (int i = 0; i < s_entity.GetChildren().size(); i++)
						{
							TransformComponent& s_tc = s_entity.GetChildren()[i].GetComponent<TransformComponent>();

							s_tc.Position -= deltaPosition;
							s_tc.Rotation -= deltaRotation;
							s_tc.Scale -= deltaScale;
						}
					}
					ImGui::EndColumns();
				}
			}

			if (m_Scene->Registry.try_get<QuadRendererComponent>(m_SelectedEntity))
			{
				QuadRendererComponent& qrc = m_Scene->Registry.get<QuadRendererComponent>(m_SelectedEntity);
				ImGui::Checkbox("##visible", &qrc.enabled);
				ImGui::SameLine();
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::CollapsingHeader("Quad Renderer"))
				{
					ImGui::BeginColumns("##quadrenderer", 2, ImGuiColumnsFlags_NoResize || ImGuiColumnsFlags_NoBorder);
					ImGui::SetColumnWidth(0, 70);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
					ImGui::Text("Scale:");
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
					ImGui::Text("Color:");
					ImGui::NextColumn();
					ImGui::ColorEdit4("##color", glm::value_ptr(qrc.Color));
					ImGui::EndColumns();
				}
			}

			if (m_Scene->Registry.try_get<PerspectiveCameraComponent>(m_SelectedEntity))
			{
				PerspectiveCameraComponent& pcc = m_Scene->Registry.get<PerspectiveCameraComponent>(m_SelectedEntity);
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::CollapsingHeader("Perspective Camera"))
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
			}


			if (m_Scene->Registry.try_get<StaticModelComponent>(m_SelectedEntity))
			{
				StaticModelComponent& smc = m_Scene->Registry.get<StaticModelComponent>(m_SelectedEntity);
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::CollapsingHeader("Static Mesh Component"))
				{
					std::string ModelPath = "-";

					if (smc.StaticModel)
						ModelPath = smc.StaticModel->GetSourceFilePath().string();
					

					ImGui::InputText("Source Path", (char*)ModelPath.c_str(), sizeof(ModelPath.c_str()));
				
					ImGui::SameLine();

					if (ImGui::Button("Load", { ImGui::CalcTextSize("Load").x + 10.0f, 20.0f }))
					{
						if (Utils::FileDialog::OpenFile("3D Model (*.*)\0*.**\0", ModelPath))
						{
							smc.StaticModel = new Model(ModelPath);
						}
					}
				}
			}

			ImGui::Separator();

			if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
			{
				ImGui::OpenPopup("ContextMenu");
			}

			if (ImGui::BeginPopup("ContextMenu"))
			{
				if (ImGui::MenuItem("Quad Renderer"))
				{
					QuadRendererComponent qrc;
					m_Scene->Registry.emplace<QuadRendererComponent>(m_SelectedEntity, qrc);
				}

				if (ImGui::MenuItem("Perspective Camera"))
				{
					PerspectiveCameraComponent pcc;
					pcc.Camera = new PerspectiveCamera(pcc.FOV, pcc.AspectRatio, pcc.NearClip, pcc.FarClip);
					m_Scene->Registry.emplace<PerspectiveCameraComponent>(m_SelectedEntity, pcc);
				}

				if (ImGui::MenuItem("Static Model"))
				{
					StaticModelComponent smc;
					m_Scene->Registry.emplace<StaticModelComponent>(m_SelectedEntity, smc);
				}

				ImGui::EndPopup();
			}


		}
		ImGui::End();
		ImGui::PopStyleVar(2);
	}
}
