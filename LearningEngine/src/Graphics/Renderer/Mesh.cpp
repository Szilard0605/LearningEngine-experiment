#include "Mesh.h"

#include "Core/Base.h"
#include "glew.h"

#include "glm.hpp"

#include "Log/Log.h"

/*Mesh::Mesh(GLVertexArray& vertexarray, GLIndexBuffer& indexbuffer, Material& material)
	: m_VertexArray(&vertexarray), m_IndexBuffer(&indexbuffer), m_Material(material)
{
	
}*/

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material)
	: m_Vertices(vertices), m_Indices(indices)
{
	m_VertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(Vertex));
	
	m_VertexBuffer->SetLayout({
		{ ShaderDataType::Float3, "a_position" }, 
		{ ShaderDataType::Float3, "a_normal"   },
		{ ShaderDataType::Float3, "a_tangent"  },
		{ ShaderDataType::Float3, "a_bitangent"},
		{ ShaderDataType::Float2, "a_texcoords"}
	});

	m_VertexArray = VertexArray::Create();
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());

	m_Material = new Material(material);
}

Mesh::~Mesh()
{

}

void Mesh::Render(PerspectiveCamera& camera, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	camera.UpdateView();

	m_Material->GetShader()->Bind();

	m_Material->GetShader()->SetMatrix4f("u_ViewProjection", camera.GetViewProjection());

	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							  * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f })
							  * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f })
							  * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f })
							  * glm::scale(glm::mat4(1.0f), scale);

	m_Material->GetShader()->SetMatrix4f("u_Transform", transform);

	m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));

	m_Material->GetTexture()->Bind(0);

	m_Material->GetShader()->SetBool("u_UseNormalMap", false);
	if (m_Material->GetNormalmap())
	{
		m_Material->GetShader()->SetBool("u_UseNormalMap", true);
		m_Material->GetNormalmap()->Bind(1);

	}

	m_VertexArray->Bind();
	m_IndexBuffer->Bind();
	m_VertexArray->DrawIndexed(m_IndexBuffer->GetCount());
}

