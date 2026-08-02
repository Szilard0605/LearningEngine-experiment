#pragma once

#include "Graphics/Renderer/API/VertexArray.h"
#include "Graphics/Renderer/API/VertexBuffer.h"
#include "Graphics/Renderer/API/IndexBuffer.h"

#include "Material.h"
#include "Core/Base.h"


class Mesh
{
public:
	
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		glm::vec2 TexCoords;
		int EntityID;
	};
	
	Mesh() = default;
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material);

	~Mesh();

	void Render();

    VertexArray* GetVertexArray() { return m_VertexArray; }
	Material* GetMaterial() { return m_Material; }
	void SetMaterial(Material* material) { m_Material = material; }

	std::vector<Vertex>& GetVertices()  { return m_Vertices; }
	std::vector<uint32_t>& GetIndices() { return m_Indices;  }

	glm::mat4& GetTransform() { return m_Transform; }
	void SetTransform(glm::mat4 transform) { m_Transform = transform; }


private:
	VertexArray* m_VertexArray;
	IndexBuffer* m_IndexBuffer;
	VertexBuffer* m_VertexBuffer;

	glm::mat4 m_Transform;

	Material* m_Material;

	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
};