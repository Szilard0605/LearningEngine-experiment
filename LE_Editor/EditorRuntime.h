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

private:
	Scene* m_Scene = nullptr;
};

