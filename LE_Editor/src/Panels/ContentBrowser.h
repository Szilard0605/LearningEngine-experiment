#pragma once

#include <filesystem>

#include "LearningEngine.h"

class ContentBrowser
{
public:
	ContentBrowser() = default;
	ContentBrowser(Scene* scene);
	

	void Render();
private:
	Scene* m_Scene = nullptr;
	
	std::filesystem::path m_CurrentDirectory;
	void DisplayDirectoryTree(std::filesystem::path directory_path);

	Texture2D* m_FileIcon = nullptr;
	Texture2D* m_FolderIcon = nullptr;
};