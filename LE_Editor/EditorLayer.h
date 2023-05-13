#pragma once

#include "LearningEngine.h"


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

private:
	Scene* m_Scene;
	Framebuffer* m_Framebuffer;
	PerspectiveCamera* m_EditorCamera;
	bool m_ViewportActive = false;
	bool m_ViewportHovered = false;

	entt::entity m_SelectedEntity;


	glm::vec2 LastMousePos = {-1, -1};
	Shader* testShader;
	Material* testMaterial;
	Model* testModel;
};

