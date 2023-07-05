#pragma once

#include "string"

#include "Material.h"
#include "Mesh.h"
#include "Graphics/Camera/PerspectiveCamera.h"

#include "glm.hpp"

#include <filesystem>

class Model
{
public:
	Model() = default;
	Model(std::filesystem::path path);
	Model(std::filesystem::path path, Material& material);

	std::vector<Mesh*>& GetMeshes() { return m_Meshes; }

	void Render(PerspectiveCamera& camera, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);

	void AddMesh(Mesh* mesh);
private:

	std::vector<Mesh*> m_Meshes;
	std::filesystem::path m_Path;
};

