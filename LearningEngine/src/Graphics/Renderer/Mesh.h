#pragma once

#include "Graphics/Renderer/API/API.h"
#include "Graphics/Camera/PerspectiveCamera.h"

#include "Material.h"
#include "Core/Base.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	

	
	Mesh() = default;
	//Mesh(GLVertexArray& vertexarray, GLIndexBuffer& indexbuffer, Material& material);
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material);
	//Mesh(Vertex vertex, uint32_t* indices, Material& material);


	~Mesh();

	void Render(PerspectiveCamera& camera, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);

    VertexArray* GetVertexArray() { return m_VertexArray; }
	Ref<Material> GetMaterial() { return m_Material; }
	void SetMaterial(const Ref<Material>& material) { m_Material = material; }

private:
	VertexArray* m_VertexArray;
	IndexBuffer* m_IndexBuffer;
	VertexBuffer* m_VertexBuffer;

	Ref<Material> m_Material;

	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
};