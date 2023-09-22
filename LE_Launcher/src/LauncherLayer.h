#pragma once
#include "LearningEngine.h"

class LauncherLayer : public Layer
{
public:
	LauncherLayer();
	virtual ~LauncherLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Event & event) override;
	virtual void OnImGuiRender() override;

	//Events
	bool OnKeyChange(KeyEvent & keyevent);
	bool OnMouseButtonChange(MouseButtonEvent & event);
	bool OnMouseMove(MouseMoveEvent & event);
	bool OnMouseScrolled(MouseScrolledEvent & event);

	void OnUpdate(Timestep timestep) override;
};

