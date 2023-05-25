#include "ContentBrowser.h"

#include "imgui.h"

static Texture2D* testImage;

ContentBrowser::ContentBrowser(Scene* scene)
	: m_Scene(scene)
{
	testImage = Texture2D::Create("res/textures/ball.png");

	m_CurrentDirectory = "res";

	m_FileIcon = Texture2D::Create("res/textures/Editor/FileIcon.jpg");
	m_FolderIcon = Texture2D::Create("res/textures/Editor/FolderIcon.jpg");

}

void ContentBrowser::Render()
{
	if (!m_Scene)
		return;

	ImGui::Begin("Content browser");
	
	// TEMPORARY UNTIL I IMPLEMENT PROJECTS
	if (ImGui::IsKeyReleased(ImGuiKey_Escape))
	{
		std::filesystem::path parent = m_CurrentDirectory.parent_path();
		std::filesystem::path base = "res";
		if(parent != base.parent_path())
			m_CurrentDirectory = parent;
	}
	/* ---------------------------------- */
	
	float padding = 16.0f;
	float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directoryEntry.path();

		Texture2D* iconTexture = directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - thumbnailSize
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		ImGui::PushID(path.filename().string().c_str());
		ImGui::ImageButton((ImTextureID)iconTexture->GetTextureID(), { 100, 100 }, { 0, 1 }, { 1, 0 });
	
		if (!directoryEntry.is_directory() && ImGui::BeginDragDropSource())
		{

			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
			{
				m_CurrentDirectory /= path.filename();
			}
		}

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (cellSize / 2.0f) - (ImGui::CalcTextSize(path.filename().string().c_str()).x / 2.0f)); 
		
		ImGui::TextWrapped(path.filename().string().c_str());

		ImGui::NextColumn();
		ImGui::PopID();

	}


	
	ImGui::End();


}
