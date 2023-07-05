#include "ContentBrowser.h"

#include "imgui.h"
#include "IconsFontAwesome5.h"
#include "Log/Log.h"
#include <imgui_internal.h>

ContentBrowser::ContentBrowser(Scene* scene)
	: m_Scene(scene)
{
	m_CurrentDirectory = "res";

	m_FileIcon = Texture2D::Create("res/textures/Editor/FileIcon.jpg");
	m_FolderIcon = Texture2D::Create("res/textures/Editor/FolderIcon.png");

}

void ContentBrowser::Render()
{
	if (!m_Scene)
		return;

	ImGui::Begin("Content browser");	

	// Dir Column
	ImGui::BeginChild("##DirColumn", { 200, 0 }, true);
	ImGui::PushItemWidth(200);

	std::filesystem::path directory_path = "res";

	DisplayDirectoryTree(directory_path);

	ImGui::PopItemWidth();
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	ImGui::SameLine();

	ImGui::BeginChild("##FileColumn", ImGui::GetContentRegionAvail(), true);

	if (ImGui::BeginPopupContextWindow("New Folder"))
	{
		static char folderName[128] = "";
		if (ImGui::InputText("Folder Name", folderName, IM_ARRAYSIZE(folderName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::string newPath = (m_CurrentDirectory / folderName).string();
			std::filesystem::create_directory(newPath);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create"))
		{
			std::string newPath = (m_CurrentDirectory / folderName).string();
			std::filesystem::create_directory(newPath);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::Text("Current Dir: ");
	ImGui::SameLine();
	ImGui::Text(m_CurrentDirectory.string().c_str());

	ImGui::Separator();

	float padding = 10.0f;
	float panelWidth = ImGui::GetContentRegionAvail().x;
	float thumbnailSize = 64;
	float cellSize = thumbnailSize + padding;

	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);
	std::filesystem::path pathToDelete = "res";

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directoryEntry.path();

		Texture2D* iconTexture = directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - thumbnailSize
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);

		ImGui::PushID(path.filename().string().c_str());
		ImVec2 size = ImVec2(thumbnailSize, thumbnailSize);
		ImVec2 uv0 = ImVec2(0.0f, 0.0f);                       
		ImVec2 uv1 = ImVec2(1, 1);
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); 
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImGui::ImageButton((ImTextureID)iconTexture->GetTextureID(), size, uv0, uv1, -1, bg_col, tint_col);		

		if (!directoryEntry.is_directory() && ImGui::BeginDragDropSource())
		{
			ImGui::EndDragDropSource();
		}
		else if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				pathToDelete = path;
			}
		}

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (thumbnailSize / 2.0f) - (ImGui::CalcTextSize(path.filename().string().c_str()).x / 2.0f));
		
		ImGui::TextWrapped(path.filename().string().c_str());

		ImGui::NextColumn();
		ImGui::PopID();
	}

	ImGui::EndChild();

	ImGui::End();
}

void ContentBrowser::DisplayDirectoryTree(std::filesystem::path directory_path) {
	if (!std::filesystem::is_directory(directory_path)) {
		std::cerr << "Error: " << directory_path << " is not a valid directory" << std::endl;
		return;
	}

	std::string directory_name = directory_path.filename().string();
	const char* imgui_folder_icon = ICON_FA_FOLDER_OPEN;
	bool open = ImGui::TreeNode(directory_name.c_str(), "%s", directory_name.c_str());
	
	if (!std::filesystem::equivalent(directory_path, "res")) {
		if (open) {
			imgui_folder_icon = ICON_FA_FOLDER_OPEN;
		}
		else {
			imgui_folder_icon = ICON_FA_FOLDER;
		}
	}
	
	if (ImGui::IsItemClicked())
	{
		m_CurrentDirectory = directory_path;
	}

	ImGui::SameLine();
	ImGui::Text(imgui_folder_icon);

	if (open) {
		for (auto& directory_entry : std::filesystem::directory_iterator(directory_path)) {
			std::filesystem::path file_path = directory_entry.path();
			if (directory_entry.is_directory()) {				
				DisplayDirectoryTree(file_path);
			}
			else if (directory_entry.is_regular_file()) {
				std::string file_name = file_path.filename().generic_string();
				ImGui::Text("%s %s", ICON_FA_FILE, file_name.c_str());
				if (ImGui::IsItemActivated() && !ImGui::IsItemHovered())
				{

				}
			}
		}

		ImGui::TreePop();
	}
}