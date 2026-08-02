#include "Model.h"

#include "Log/Log.h"
#include "Graphics/Renderer/ShaderLibrary.h"

void Model::Render(PerspectiveCamera& camera, glm::mat4 transform)
{
	for(int i = 0; i < m_Meshes.size(); i ++)
	{
		m_Meshes[i].Render();
	}
}

void Model::AddMesh(Mesh mesh)
{
	m_Meshes.push_back(mesh);
}
