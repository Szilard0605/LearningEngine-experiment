#pragma once

#include "LearningEngine.h"

class DemoLayer : public Layer
{
public:
	DemoLayer();
	virtual ~DemoLayer() = default;

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

	PerspectiveCamera Camera;

	static constexpr glm::vec4 clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

	Material demoMaterial;
	Model demoModel;

	glm::vec2 LastMousePos = { -1, -1 };
	glm::mat4 cubeTransform;

	glm::vec4 u_Color;
	glm::vec3 u_LightPosition = { 1, 3000, 1 };
	float u_LightAttenuation = 0.0f;
	glm::vec4 u_LightColor = { 1, 1, 1, 1 };;
	glm::vec3 u_LightDir = { 1, 1, 1 };
};

