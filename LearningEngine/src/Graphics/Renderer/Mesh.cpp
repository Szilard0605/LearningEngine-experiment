#include "Mesh.h"

#include "Core/Base.h"
#include "glew.h"

#include "glm.hpp"

#include "Log/Log.h"

#include "ForwardRenderer.h"

/*Mesh::Mesh(GLVertexArray& vertexarray, GLIndexBuffer& indexbuffer, Material& material)
	: m_VertexArray(&vertexarray), m_IndexBuffer(&indexbuffer), m_Material(material)
{
	
}*/

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material)
	: m_Vertices(vertices), m_Indices(indices)
{
	m_VertexBuffer = VertexBuffer::Create(static_cast<uint32_t>(vertices.size() * sizeof(Vertex)));
	
	m_VertexBuffer->SetLayout({
		{ ShaderDataType::Float3, "a_position" }, 
		{ ShaderDataType::Float3, "a_normal"   },
		{ ShaderDataType::Float3, "a_tangent"  },
		{ ShaderDataType::Float3, "a_bitangent"},
		{ ShaderDataType::Float2, "a_texcoords"}
	});

	m_VertexArray = VertexArray::Create();
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	m_IndexBuffer = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));

	m_Material = new Material(material);
}

Mesh::~Mesh()
{

}

void Mesh::Render(PerspectiveCamera& camera, glm::mat4 transform)
{
	camera.UpdateView();

	m_Material->GetShader()->Bind();

	m_Material->GetShader()->SetMatrix4f("u_ViewProjection", camera.GetViewProjection());

	m_Material->GetShader()->SetMatrix4f("u_Transform", transform);

	m_VertexBuffer->SetData(m_Vertices.data(), static_cast<uint32_t>(m_Vertices.size() * sizeof(Vertex)));

	if (m_Material->GetTexture())
	{
		m_Material->GetTexture()->Bind(0);
	}


	m_VertexArray->Bind();
	m_IndexBuffer->Bind();
	m_VertexArray->DrawIndexed(m_IndexBuffer->GetCount());
}

