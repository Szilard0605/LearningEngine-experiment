#pragma once

#include <imgui.h>
#include "entt"

class HierarchyWindow {
public:
    HierarchyWindow(entt::registry& registry) : m_Registry(registry) {}

    void DrawWindow() {
        ImGui::Begin("Hierarchy");

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        DrawEntity(m_Registry, m_Registry, m_Registry.get<entt::entity>(m_Registry.entity(entityID)));

        if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
            ImGui::OpenPopup("Create Entity");
        }

        if (ImGui::BeginPopup("Create Entity")) {
            if (ImGui::MenuItem("Create Entity"))
            {
                auto entity = m_Registry.create();
                

                entt::entity parent = m_Registry.get<entt::entity>(m_Registry.entity(entityID));
                if (parent == entt::null) {
                    parent = m_Registry.create();
                    parent.assign<entt::tag<"HierarchyRoot"_hs>>();
                    m_Registry.emplace<entt::entity>(m_Registry.entity(entityID), parent);
                }

                m_Registry.emplace<entt::entity>(entity, parent);
            }
            ImGui::EndPopup();
        }

        ImGui::EndChild();

        ImGui::End();
    }

private:
    void DrawEntity(entt::registry& registry, entt::registry& rootRegistry, entt::entity entity) {
        std::string name = "Entity " + std::to_string(static_cast<uint32_t>(entity));
        if (registry.has<entt::tag<"Name"_hs>>(entity)) {
            auto& tag = registry.get<entt::tag<"Name"_hs>>(entity);
            name = std::string(tag.value.data(), tag.value.size());
        }

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (m_SelectedEntity == entity)
            flags |= ImGuiTreeNodeFlags_Selected;

        bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uintptr_t>(entity)),
            flags, name.c_str());

        if (ImGui::IsItemClicked())
            m_SelectedEntity = entity;

        bool node_deleted = false;
        bool context_clicked = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                node_deleted = true;
            if (ImGui::MenuItem("Create Entity"))
                context_clicked = true;
            ImGui::EndPopup();
        }

        if (opened) {
            auto view = registry.view<entt::entity>();
            for (auto child : view) {
                if (registry.has<entt::entity>(child)) {
                    entt::entity parent = registry.get<entt::entity>(child);
                    if (parent == entity)
                        DrawEntity(registry, rootRegistry, child);
                }
            }

            ImGui::TreePop();
        }

        // If a node was clicked on and deleted, delete the entire node hierarchy
        if (node_deleted) {
            auto& parent = m_Registry.get<entt::entity>(rootRegistry.entity(m_SelectedEntity));
            m_Registry.destroy(entity); // destroys the entity and all its children

            m_SelectedEntity = entt::null;
        }

        // If the right click context menu was clicked on, create a new entity
        if (context_clicked) {
            auto entity = m_Registry.create();
            entity.assign<entt::tag<"Entity"_hs>>();
            m_Registry.emplace<entt::entity>(entity, m_SelectedEntity);
        }
    }

    entt::registry& m_Registry;
    entt::entity m_SelectedEntity = entt::null;
};
