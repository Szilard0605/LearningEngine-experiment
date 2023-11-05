#include "MeshFactory.h"

#include "glew.h"

Mesh* MeshFactory::CreateCube(float size, Material& material)
{
	Mesh::Vertex data[8];

	std::vector<Mesh::Vertex> vertices;

	vertices.reserve(8 * sizeof(Mesh::Vertex));

	data[0].Position = glm::vec3(0.5f, 0.5f, 0.5f);
	data[1].Position = glm::vec3(-0.5f, 0.5f, -0.5f);
	data[2].Position = glm::vec3(-0.5f, 0.5f, 0.5f);
	data[3].Position = glm::vec3(0.5f, -0.5f, -0.5f);
	data[4].Position = glm::vec3(-0.5f, -0.5f, -0.5f);
	data[5].Position = glm::vec3(0.5f, 0.5f, -0.5f);
	data[6].Position = glm::vec3(0.5f, -0.5f, 0.5f);
	data[7].Position = glm::vec3(-0.5f, -0.5f, 0.5f);

	data[0].Normal = glm::vec3(-1.0f, -1.0f, 1.0f);
	data[1].Normal = glm::vec3(1.0f, -1.0f, 1.0f);
	data[2].Normal = glm::vec3(1.0f, 1.0f, 1.0f);
	data[3].Normal = glm::vec3(-1.0f, 1.0f, 1.0f);
	data[4].Normal = glm::vec3(-1.0f, -1.0f, -1.0f);
	data[5].Normal = glm::vec3(1.0f, -1.0f, -1.0f);
	data[6].Normal = glm::vec3(1.0f, 1.0f, -1.0f);
	data[7].Normal = glm::vec3(-1.0f, 1.0f, -1.0f);

	glm::vec2 tcBottomLeft(0.0f, 0.0f);
	glm::vec2 tcTopLeft(0.0f, 1.0f);
	glm::vec2 tcBottomRight(1.0f, 0.0f);
	glm::vec2 tcTopRight(1.0f, 1.0f);

	data[0].TexCoords = glm::vec2(1.0f, 0.0f);
	data[1].TexCoords = glm::vec2(0.0f, 1.0f);
	data[2].TexCoords = glm::vec2(1.0f, 1.0f);
	data[3].TexCoords = glm::vec2(0.0f, 0.0f);
	data[4].TexCoords = glm::vec2(1.0f, 0.0f);
	data[5].TexCoords = glm::vec2(0.0f, 1.0f);
	data[6].TexCoords = glm::vec2(1.0f, 1.0f);
	data[7].TexCoords = glm::vec2(0.0f, 0.0f);

	for (int i = 0; i < 8; i++)
		vertices.push_back(data[i]);

	uint32_t* indices = new uint32_t[36]
	{
			0, 1, 2,
			1, 3, 4,
			5, 6, 3,
			7, 3, 6,
			2, 4, 7,
			0, 7, 6,
			0, 5, 1,
			1, 5, 3,
			5, 0, 6,
			7, 4, 3,
			2, 1, 4,
			0, 2, 7
	};

	std::vector<uint32_t> mesh_indices;
	for (int i = 0; i < 36; i++)
	{
		mesh_indices.push_back(indices[i]);
	}
		

	return new Mesh(vertices, mesh_indices, material);
}
