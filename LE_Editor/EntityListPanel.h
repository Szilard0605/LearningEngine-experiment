#pragma once

#include "entt.hpp"

class Scene;

class EntityListPanel
{
public:
	EntityListPanel() = default;
	EntityListPanel(Scene* scene);

	void Render();
private:

	void DisplayHierarchy(entt::entity entity);

	Scene* m_Scene = nullptr;

	bool m_DeleteEntity = false;
	bool m_Enable = true;

	entt::entity m_SelectedEntity = entt::null;
};

