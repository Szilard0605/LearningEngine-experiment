#include "EditorLayer.h"

#include "imgui.h"
#include "Log/Log.h"

// GLM
#include "gtc/type_ptr.hpp"

glm::vec2 EditorLayer::s_MainViewportSize;

EditorLayer::EditorLayer() : Layer("LearningEngine Editor")
{
	LE_CORE_INFO("Editor started");
}

void EditorLayer::OnAttach()
{
	LE_CORE_INFO("Editor attached");

	m_Scene = new Scene("OtherScene");

	Framebuffer::FramebufferSpecifications specs;
	specs.Attachments = { Framebuffer::FramebufferTextureFormat::RGBA8,Framebuffer::FramebufferTextureFormat::RED_INTEGER, Framebuffer::FramebufferTextureFormat::Depth };
	specs.Width = 1280;
	specs.Height = 720;
	m_Framebuffer = Framebuffer::Create(specs);

	s_MainViewportSize = { specs.Width, specs.Height };

	m_EditorCamera = new PerspectiveCamera(60.0f, 1280.f / 720.0f, 1.0f, 10000.0f);
	m_Scene->SetMainCamera(m_EditorCamera);

	m_EntitiesPanel = EntityListPanel(m_Scene);
	m_ContentBrowser = ContentBrowser(m_Scene);

	m_TexPlayButton = Texture2D::Create("res/textures/Editor/play_button.png");

	ShaderLibrary::Add(Shader::Create("res/shaders/default_shader.shader"), "DefaultShader");
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnEvent(Event& event)
{
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
		ImGui::Begin("Viewport");

		ImGui::BeginGroup();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2.0f);
		ImVec4 playbtn_col = m_PressedPlay ? ImVec4(0.0f, 0.0f, 1.0f, 0.3f) : ImVec4(1.0f, 1.0f, 1.0f, 0.3f);

		ImGui::PushStyleColor(ImGuiCol_Button, playbtn_col);
		if (ImGui::ImageButton((void*)m_TexPlayButton->GetTextureID(), { 20, 20 }, { 0, 1 }, { 1, 0 }))
		{
			m_PressedPlay = !m_PressedPlay;

			if (m_PressedPlay)
			{
				m_Runtime.Start(m_Scene);
			}
			else
			{
				m_Runtime.Stop();
			}
		}
		ImGui::PopStyleColor();

		ImGui::PopItemWidth();
		ImGui::EndGroup();

		float viewportWidth = ImGui::GetContentRegionAvail().x;
		float viewportHeight = ImGui::GetContentRegionAvail().y;

		uint32_t texid = m_Framebuffer->GetColorAttachmentID(0);

		Framebuffer::FramebufferSpecifications fbSpecs = m_Framebuffer->GetSpecification();
		if (fbSpecs.Width != viewportWidth || fbSpecs.Height != viewportHeight)
		{
			m_Framebuffer->Resize(viewportWidth, viewportHeight);
			m_EditorCamera->SetAspectRatio(viewportWidth / viewportHeight);

			s_MainViewportSize = {viewportWidth, viewportHeight};
		}

		ImGui::Image((void*)texid, ImVec2(viewportWidth, viewportHeight));

		m_ViewportActive = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImGui::End();
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save scene"))
			{
				SceneSerializer::Serialize(m_Scene);
			}

			if (ImGui::MenuItem("Load scene"))
			{
				LE_CLIENT_INFO("Loading a Scene");

				std::string ScenePath;
				if (Utils::FileDialog::OpenFile("LearingEngine Scene (*.lescene)\0*.lescene*\0", ScenePath))
				{
					m_Scene = SceneSerializer::Load(ScenePath);
					m_EntitiesPanel = EntityListPanel(m_Scene);
					m_ContentBrowser = ContentBrowser(m_Scene);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	


	ImGui::End();

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

		PerspectiveCamera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Scene->Registry.view<TransformComponent, PerspectiveCameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, PerspectiveCameraComponent>(entity);


				Renderer2D::Begin(*m_EditorCamera);

				Renderer2D::DrawQuad(transform.Position, { 5, 5, 5}, { 0, 0, 0 }, {0, 0, 1, 1});

				Renderer2D::End();
			}
		}


		if (m_ViewportActive)
		{
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

	m_Framebuffer->Unbind();
}
