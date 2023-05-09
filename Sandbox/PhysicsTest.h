#pragma once

#include "LearningEngine.h"

class PhysicsTest : public Layer
{
public:
	PhysicsTest();
	virtual ~PhysicsTest() = default;

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
	
	Shader* testShader;
	Model testModel;
	Material testMaterial;
	Material testMaterial2;
};

