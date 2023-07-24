#pragma once

#include "LearningEngine.h"

#include "EntityListPanel.h"
#include "ContentBrowser.h"
#include "EditorRuntime.h"

class EditorLayer : public Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Event& event) override;
	virtual void OnImGuiRender() override;
	
	//Events
	bool OnKeyChange(KeyEvent& keyevent);
	bool OnMouseButtonChange(MouseButtonEvent& event);
	bool OnMouseMove(MouseMoveEvent& event);
	bool OnMouseScrolled(MouseScrolledEvent& event);

	void OnUpdate(Timestep timestep) override;
	static glm::vec2 GetMainViewportSize() { return s_MainViewportSize; };

private:

	Scene* m_Scene;
	Framebuffer* m_Framebuffer;
	PerspectiveCamera* m_EditorCamera;
	bool m_ViewportActive = false;
	bool m_ViewportHovered = false;

	static glm::vec2 s_MainViewportSize;

	glm::vec2 LastMousePos = {-1, -1};

	// Panels
	EntityListPanel m_EntitiesPanel;
	ContentBrowser m_ContentBrowser;

	Texture2D* m_TexPlayButton;
	bool m_PressedPlay = false;

	EditorRuntime m_Runtime;
};

