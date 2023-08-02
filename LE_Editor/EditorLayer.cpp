#include "EditorLayer.h"

// might need to change this
#include "ImGuizmo.cpp"
#include "ImGuizmo.h"


#include "imgui.h"
#include "Log/Log.h"

// GLM
#include "gtc/type_ptr.hpp"
#include <imgui_internal.h>

#include "gtx/matrix_decompose.hpp"

glm::vec2 EditorLayer::s_MainViewportSize;

EditorLayer::EditorLayer() : Layer("LearningEngine Editor")
{
	LE_CORE_INFO("Editor started");
}

void EditorLayer::OnAttach()
{
	LE_CORE_INFO("Editor attached");

	m_Scene = new Scene("Scene");

	Framebuffer::FramebufferSpecifications specs;
	specs.Attachments = { Framebuffer::FramebufferTextureFormat::RGBA8,Framebuffer::FramebufferTextureFormat::RED_INTEGER, Framebuffer::FramebufferTextureFormat::Depth };
	specs.Width = 1280;
	specs.Height = 720;
	m_Framebuffer = Framebuffer::Create(specs);

	s_MainViewportSize = { specs.Width, specs.Height };

	m_EditorCamera = new PerspectiveCamera(60.0f, specs.Width / specs.Height, 1.0f, 10000.0f);
	m_Scene->SetMainCamera(m_EditorCamera);
	m_Scene->OnViewportResize(s_MainViewportSize.x, s_MainViewportSize.y);

	m_EntitiesPanel = EntityListPanel(m_Scene);
	m_ContentBrowser = ContentBrowser(m_Scene);

	/* ---- Loading Assets ------
	 Note (Szilard): We should load registry from the project 
	 directory when projects are implemented */
	AssetManager::LoadAssetsFromRegistry("res/AssetRegistry.lereg");

	m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

	//---------------------------
	m_TexPlayButton = Texture2D::Create("res/textures/Editor/play_button.png");
	m_TranslateIcon = Texture2D::Create("res/textures/Editor/icon_translate.png");
	m_RotateIcon = Texture2D::Create("res/textures/Editor/icon_rotate.png");
	m_ScaleIcon = Texture2D::Create("res/textures/Editor/icon_scale.png");

	// Setting the default shader of the project
	ShaderLibrary::Add(Shader::Create("res/shaders/default_shader.shader"), "DefaultShader");
}

void EditorLayer::OnDetach()
{
	/* ---- Saving Assets ------
	 Note (Szilard): We should save the assets into registry 
	 when project saved when projects are implemented */
	AssetManager::SaveAllAssets("res/AssetRegistry.lereg");
}

void EditorLayer::OnEvent(Event& event)
{
	event.Dispatch<KeyEvent>(BIND_EVENT_FN(EditorLayer::OnKeyChange));
	event.Dispatch<MouseMoveEvent>(BIND_EVENT_FN(EditorLayer::OnMouseMove));
	event.Dispatch<MouseButtonEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonChange));
}

void EditorLayer::OnImGuiRender()
{
	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	bool showSavePopup = false;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PopStyleVar();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PopStyleVar();
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();



	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	style.FramePadding = ImVec2(4.0f, 4.0f);
	style.WindowBorderSize = 0.0f;
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.WindowPadding = ImVec2(5.0f, 5.0f);
	style.ChildBorderSize = 0.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	m_EntitiesPanel.Render();
	m_ContentBrowser.Render();


	//Viewport window
	{

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::Begin("##Viewport", nullptr, ImGuiWindowFlags_NoTitleBar);

		float viewportWidth = ImGui::GetContentRegionAvail().x;
		float viewportHeight = ImGui::GetContentRegionAvail().y;

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();

		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


		uint32_t texid = m_Framebuffer->GetColorAttachmentID(0);

		Framebuffer::FramebufferSpecifications fbSpecs = m_Framebuffer->GetSpecification();
		if (fbSpecs.Width != viewportWidth || fbSpecs.Height != viewportHeight)
		{
			m_Framebuffer->Resize((uint32_t)viewportWidth, (uint32_t)viewportHeight);
			m_EditorCamera->SetAspectRatio(viewportWidth / viewportHeight);
			m_Scene->OnViewportResize(viewportWidth, viewportHeight);
			s_MainViewportSize = { viewportWidth, viewportHeight };
		}

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		//vpSize = { viewportPanelSize.x, viewportPanelSize.y };

		ImGui::Image(reinterpret_cast<void*>(texid), viewportPanelSize, { 0, 1 }, {1, 0});

		ImGui::SameLine();

		// Rendering the gizmo type selector
		ImGui::SetCursorPos({0, 0});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0, 0, 0, 0});
		ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.0f, 0.0f, 0.0f });

		float opButtonSize = 30;
		float opPadding = 2.0f;

		bool translateHovered = false;
		bool rotateHovered = false;
		bool scaleHovered = false;

		ImVec4 tintColor = m_GizmoType == ImGuizmo::OPERATION::TRANSLATE ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.5f);

		if (ImGui::ImageButton(reinterpret_cast<void*>(m_TranslateIcon->GetTextureID()), { opButtonSize, opButtonSize }, { 0, 0 }, { 1, 1 }, -1, { 0, 0, 0, 0 }, tintColor))
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

		translateHovered = ImGui::IsItemHovered();
		
		tintColor = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.5f);
		
		ImGui::SetCursorPos({ opButtonSize + opPadding, 0 });
		if (ImGui::ImageButton(reinterpret_cast<void*>(m_RotateIcon->GetTextureID()), { opButtonSize, opButtonSize }, { 0, 0 }, { 1, 1 }, -1, { 0, 0, 0, 0 }, tintColor))
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;

		rotateHovered = ImGui::IsItemHovered();

		tintColor = m_GizmoType == ImGuizmo::OPERATION::SCALE ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.5f);

		ImGui::SetCursorPos({ opButtonSize * 2 + opPadding, 0 });
		if (ImGui::ImageButton(reinterpret_cast<void*>(m_ScaleIcon->GetTextureID()), { opButtonSize, opButtonSize }, { 0, 1 }, { 1, 0 }, -1, { 0, 0, 0, 0 }, tintColor))
			m_GizmoType = ImGuizmo::OPERATION::SCALE;

		scaleHovered = ImGui::IsItemHovered();

		m_OperationIconHovered = translateHovered || rotateHovered || scaleHovered;

		ImGui::PopStyleColor(3);
		//-----------------------------------------

		m_ViewportActive = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		

	
		UpdateGizmos();

		ImGui::End();
		ImGui::PopStyleVar();

		//ImGui::PopItemWidth();
	}
	
	// Tool bar
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::SetNextWindowClass(&window_class);
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2.0f);
		ImVec4 playbtn_col = m_PressedPlay ? ImVec4(0.0f, 0.0f, 1.0f, 0.3f) : ImVec4(1.0f, 1.0f, 1.0f, 0.3f);

		ImGui::PushStyleColor(ImGuiCol_Button, playbtn_col);
		if (ImGui::ImageButton((void*)m_TexPlayButton->GetTextureID(), { 20, 20 }, { 0, 1 }, { 1, 0 }))
		{
			m_PressedPlay = !m_PressedPlay;

			if (m_PressedPlay)
			{
				m_Runtime.Start(m_Scene);
				m_EntitiesPanel.SetScene(m_Runtime.GetScene());
			}
			else
			{
				m_Runtime.Stop();
				m_EntitiesPanel.SetScene(m_Scene);
			}
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
		ImGui::End();

	}	

	if (ImGui::IsKeyPressed(ImGuiKey_C)) {
		ImGui::OpenPopup("Save");
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{

			}
			
			if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
			{
				LE_CLIENT_INFO("Loading a Scene");

				std::string ScenePath;
				if (Utils::FileDialog::OpenFile("LearningEngine Scene (*.lescene)\0*.lescene*\0", ScenePath))
				{
					m_Scene = SceneSerializer::Load(ScenePath);
					m_EntitiesPanel = EntityListPanel(m_Scene);
					m_ContentBrowser = ContentBrowser(m_Scene);
					if (m_PressedPlay)
					{
						m_Runtime.Stop();
						m_PressedPlay = false;
					}
				}
			}

			if (ImGui::MenuItem("Save As...", "Ctrl+S"))
			{
				LE_CLIENT_INFO("Saving a Scene");

				std::string scenePath = Utils::FileDialog::SaveFile("LearningEngine Scene (*.lescene)\0.lescene\0");

				if (!scenePath.empty())
				{
					SceneSerializer::Serialize(m_Scene, scenePath);
				}
			}			

			ImGui::MenuItem("Exit", "Alt+F4");
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	
	ImGui::End();

}

void EditorLayer::UpdateGizmos()
{
	entt::entity selectedEntity = m_EntitiesPanel.GetSelectedEntity();

	if (m_Scene->Registry.valid(selectedEntity))
	{

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

		// Editor camera
		const glm::mat4& cameraProjection = m_EditorCamera->GetProjectionMatrix();
		glm::mat4 cameraView = m_EditorCamera->GetViewMatrix();

		// Entity transform
		//auto& tc = selectedEntity.GetComponent<TransformComponent>();
		auto& tc = m_Scene->Registry.get<TransformComponent>(selectedEntity);
		glm::mat4 transform = tc.GetTransform();

		// Snapping
		bool snap = Input::IsKeyPressed(Key::LeftControl);
		float snapValue = 0.5f; // Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			snapValue = 45.0f;

		float snapValues[3] = { snapValue, snapValue, snapValue };

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
			(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
			nullptr, snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, scale;
			glm::quat orientation;
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(transform, scale, orientation, translation, skew, perspective);
			// Convert the quaternion to vec3 (Euler angles)
			glm::vec3 rotation = glm::eulerAngles(orientation);

			//printf("eulerAngle: {%f, %f, %f}\n", eulerRotation.x, eulerRotation.y, eulerRotation.z);

			glm::vec3 deltaRotation = rotation - tc.Rotation;
			tc.Position = translation;
			tc.Rotation += deltaRotation;
			tc.Scale = scale;
		}
	}
}

bool EditorLayer::OnKeyChange(KeyEvent& keyevent)
{ 

	return false;
}

bool EditorLayer::OnMouseButtonChange(MouseButtonEvent& event)
{
	if (event.GetButton() == MouseButton::RIGHT_CLICK && event.GetAction() == KeyAction::RELEASED)
	{
		LastMousePos = { -1, -1 };
	}
		
	if (event.GetButton() == MouseButton::LEFT_CLICK && event.GetAction() == KeyAction::PRESSED &&
		m_ViewportHovered && !ImGuizmo::IsOver() && !m_OperationIconHovered)
	{
		if (m_Scene->Registry.valid(m_HoveredEntity))
		{
			m_EntitiesPanel.SetSelectedEntity(m_HoveredEntity);
		}
		else
		{
			m_EntitiesPanel.SetSelectedEntity(entt::null);
		}
	}

	return true;
}

bool EditorLayer::OnMouseMove(MouseMoveEvent& event)
{
	if (Input::IsMouseButtonPressed(MouseButton::RIGHT_CLICK) && m_ViewportActive && m_ViewportHovered)
	{
		if (LastMousePos == glm::vec2(-1, -1))
			LastMousePos = event.GetPosition();

		m_EditorCamera->SetYaw(m_EditorCamera->GetYaw() + (event.GetPosition().x - LastMousePos.x) * 0.01f);
		m_EditorCamera->SetPitch(m_EditorCamera->GetPitch() - (event.GetPosition().y - LastMousePos.y) * 0.01f);

		LastMousePos = event.GetPosition();
	}
	return true;
}

bool EditorLayer::OnMouseScrolled(MouseScrolledEvent& event)
{
	return false;
}

void EditorLayer::OnUpdate(Timestep timestep)
{
	m_Framebuffer->Bind();

	Renderer2D::ClearColor(glm::vec4(0.5, 0.5, 0.5, 1));




	if (m_PressedPlay)
	{
		m_Runtime.Update(timestep);
	}
	else
	{
		m_Scene->Render(m_EditorCamera);

		/*PerspectiveCamera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Scene->Registry.view<TransformComponent, PerspectiveCameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, PerspectiveCameraComponent>(entity);


				Renderer2D::Begin(*m_EditorCamera);

				Renderer2D::DrawQuad(transform.Position, { 5, 5, 5}, { 0, 0, 0 }, {0, 0, 1, 1}, entity);

				Renderer2D::End();
			}
		}*/


		if (m_ViewportActive)
		{

			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				m_HoveredEntity = pixelData == -1 ? entt::null : (entt::entity)pixelData;

				if (m_Scene->Registry.valid(m_HoveredEntity))
				{
					TagComponent& tc = m_Scene->Registry.get<TagComponent>(m_HoveredEntity);
				}
			}

			float speed = 0.1f;

			if (Input::IsKeyPressed(Key::LeftShift))
				speed *= 2;

			if (Input::IsKeyPressed(Key::W))
			{
				m_EditorCamera->Translate(m_EditorCamera->GetPosition() + (speed * m_EditorCamera->GetForwardDirection()));
			}

			if (Input::IsKeyPressed(Key::S))
			{
				m_EditorCamera->Translate(m_EditorCamera->GetPosition() - (speed * m_EditorCamera->GetForwardDirection()));
			}

			if (Input::IsKeyPressed(Key::D))
			{
				m_EditorCamera->Translate(m_EditorCamera->GetPosition() + (speed * m_EditorCamera->GetRightDirection()));
			}
			if (Input::IsKeyPressed(Key::A))
			{
				m_EditorCamera->Translate(m_EditorCamera->GetPosition() - (speed * m_EditorCamera->GetRightDirection()));
			}
		}
	}


	//printf("Hovered entity: %d\n", (int)m_HoveredEntity);

	m_Framebuffer->Unbind();
}
