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
	Model(Model& other) {
		m_Meshes = other.m_Meshes;
		m_SourcePath = other.m_SourcePath;
	}
	Model(std::vector<Mesh> meshes) {
		m_Meshes = meshes;
	}

	std::vector<Mesh>& GetMeshes() { return m_Meshes; }

	void SetSourcePath(std::filesystem::path path) { m_SourcePath = path; }
	std::filesystem::path GetSourcePath() { return m_SourcePath; }
	void Render(PerspectiveCamera& camera, glm::mat4 transform);
	void AddMesh(Mesh mesh);
private:
	std::vector<Mesh> m_Meshes;
	std::filesystem::path m_SourcePath;
};

