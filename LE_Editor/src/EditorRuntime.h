#pragma once

#include "LearningEngine.h"

class EditorRuntime
{
public:
	EditorRuntime();
	~EditorRuntime();

	void Start(Scene* scene);
	void Stop();
	void Update(Timestep timeStep);

	Scene* GetScene() { return m_Scene; }

private:
	Scene* m_Scene = nullptr;
};

