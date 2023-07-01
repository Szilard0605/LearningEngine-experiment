#pragma once

#include "entt/entt.hpp"

class Scene;

class EntityListPanel
{
public:
	EntityListPanel() = default;
	EntityListPanel(Scene* scene);

	void Render();
private:
	Scene* m_Scene = nullptr;

	bool m_DeleteEntity = false;

	entt::entity m_SelectedEntity = entt::null;
};

