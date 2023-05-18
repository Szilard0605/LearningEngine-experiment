#pragma once

class Scene;

class ContentBrowser
{
public:
	ContentBrowser() = default;
	ContentBrowser(Scene* scene);

	void Render();
private:
	Scene* m_Scene = nullptr;
};

