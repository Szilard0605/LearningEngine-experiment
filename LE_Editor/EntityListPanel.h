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
	Scene* m_Scene = nullptr;

	entt::entity m_SelectedEntity = entt::null;
};

