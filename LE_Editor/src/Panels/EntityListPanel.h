#pragma once

#include "entt.hpp"
#include "Graphics/Scene/Entity.h"

class Scene;

class EntityListPanel
{
public:
	EntityListPanel() = default;
	EntityListPanel(Scene* scene);

	void Render();
	void SetScene(Scene* scene);
	Scene* GetScene() { return m_Scene; }

	void DeleteEntity(Entity entity);
	void ShowDeleteEntityPopup() { m_DeleteEntityPopup = true; }

	entt::entity GetSelectedEntity() { return m_SelectedEntity; }
	void SetSelectedEntity(entt::entity entity);
private:
	void DisplayHierarchy(Entity entity);

	bool m_DeleteEntityPopup = false;

	Scene* m_Scene = nullptr;

	bool m_Enable = true;

	entt::entity m_SelectedEntity = entt::null;
};

