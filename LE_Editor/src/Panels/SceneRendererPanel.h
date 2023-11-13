#pragma once

#include "LearningEngine.h"

class SceneRendererPanel
{
public:
	SceneRendererPanel() = default;
	SceneRendererPanel(Scene* scene);
	~SceneRendererPanel();

	void Render();

private:
	Scene* m_Scene;
};

