#pragma once


#include "Graphics/Renderer/Material.h"
#include "EntityListPanel.h"

class MaterialInspectorPanel
{
public:
	MaterialInspectorPanel() = default;
	MaterialInspectorPanel(EntityListPanel* entitylistpanel);
	void Render();

private:
	EntityListPanel* m_elpRef; // for accessing selected entites
};

