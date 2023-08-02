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

	entt::entity GetSelectedEntity() { return m_SelectedEntity; }
	void SetSelectedEntity(entt::entity entity) { m_SelectedEntity = entity; }
private:

	void DisplayHierarchy(Entity& entity);

	Scene* m_Scene = nullptr;

	bool m_DeleteEntity = false;
	bool m_Enable = true;

	entt::entity m_SelectedEntity = entt::null;
};

