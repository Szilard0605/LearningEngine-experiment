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
	Model(std::filesystem::path path, Material material);

	std::vector<Mesh>& GetMeshes() { return m_Meshes; }

	void Render(PerspectiveCamera& camera, glm::mat4 transform);
	void AddMesh(Mesh mesh);

	std::filesystem::path GetSourceFilePath() { return m_Path; }
private:

	std::vector<Mesh> m_Meshes;
	std::filesystem::path m_Path;
};

